#include "mainwindow.h"

#include "./ui_mainwindow.h"

#include "DXFGraphicsScene.h"

#include "DXFReader.h"

#include "config.h"

#include <QMessageBox>
#include <QDesktopServices>
#include <QSettings>
#include <QFileDialog>
#include <QCommandLineParser>
#include <QTimer>
#include <QPrinter>
#include <QPrintDialog>

#include "spdlog/spdlog.h"

#include "spdlog/sinks/qt_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"

constexpr float SCALE = 150;

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	QCoreApplication::setApplicationName(PROJECT_NAME);
	QCoreApplication::setApplicationVersion(PROJECT_VER);
	ui->setupUi(this);

	auto const log_name{ "log.txt" };

	appdir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	std::filesystem::create_directory(appdir.toStdString());
	QString const logdir = appdir + "/log/";
	std::filesystem::create_directory(logdir.toStdString());

	try
	{
		auto file{ std::string(logdir.toStdString() + log_name) };
		auto rotating = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(file, 1024 * 1024, 5, false);

		logger = std::make_shared<spdlog::logger>("xdxfviewer", rotating);
		logger->flush_on(spdlog::level::debug);
		logger->set_level(spdlog::level::debug);
		logger->set_pattern("[%D %H:%M:%S] [%L] %v");
	}
	catch (std::exception& /*ex*/)
	{
		QMessageBox::warning(this, "Logger Failed", "Logger Failed To Start.");
	}

	//todo
	ui->le_Search->setVisible(false);
	ui->pb_Search->setVisible(false);

	//logger->info("Program started v" + std::string( PROJECT_VER));
	//logger->info(std::string("Built with Qt ") + std::string(QT_VERSION_STR));
	dxf_scene = std::make_unique<DXFGraphicsScene>();
	ui->graphicsView->setScene(dxf_scene.get());

	setWindowTitle(windowTitle() + " v" + PROJECT_VER);

	settings = std::make_unique< QSettings>(appdir + "/settings.ini", QSettings::IniFormat);

	QTimer::singleShot(500, this, SLOT(ProcessCommandLine()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_actionOpen_DXF_triggered()
{
	QString const dxfFile = QFileDialog::getOpenFileName(this, "Select DXF File", settings->value("last_dxf").toString(), tr("DXF Files (*.dxf);;All Files (*.*)"));
	if (!dxfFile.isEmpty())
	{
		OpenDXF(dxfFile);
		settings->setValue("last_dxf", dxfFile);
		settings->sync();
	}
}

void MainWindow::on_actionPrint_triggered()
{
	//https://stackoverflow.com/questions/3803170/how-to-print-content-of-a-qgraphicsview
	QPrinter printer;
	if (QPrintDialog(&printer).exec() == QDialog::Accepted)
	{
		QPainter painter(&printer);
		painter.setRenderHint(QPainter::Antialiasing);
		dxf_scene->render(&painter);
		statusBar()->showMessage("Print Successful", 3000);
	}
}

void MainWindow::on_actionClose_triggered()
{
	close();
}

void MainWindow::on_actionAbout_triggered()
{
	auto hpp{ helpText.right(helpText.size() - helpText.indexOf("Options:")) };
	QString text{ QString("About xDXF Viewer v%1\nQT v%2\n\n\nCommandline %3\n\nIcons by:\n%4")
		.arg(PROJECT_VER).arg(QT_VERSION_STR)
		.arg(hpp)
		.arg(R"(http://www.famfamfam.com/lab/icons/silk/)") };
	//http://www.famfamfam.com/lab/icons/silk/
	QMessageBox::about(this, "About xDXF Viewer", text);
}

void MainWindow::on_actionOpen_Logs_triggered()
{
	QDesktopServices::openUrl(QUrl::fromLocalFile(appdir + "/log/"));
}

void MainWindow::OpenDXF(QString const& file)
{
	logger->debug("Reading \'" + file.toStdString() + "\'");
	DXFReader dxf_data(file.toStdString());

	for (auto const& text : dxf_data.getText())
	{
		drawText(text);
	}

	for (auto const& line : dxf_data.getLines())
	{
		drawLine(line);
	}

	for (auto const& circle : dxf_data.getCircles())
	{
		drawCircle(circle);
	}
	ui->graphicsView->fitInView(ui->graphicsView->sceneRect(), Qt::KeepAspectRatio);
	statusBar()->showMessage("Loaded: " + file, 3000);
}

void MainWindow::drawText( DXFText const& text)
{
	QColor drawColor(Qt::black);
	if(text.color24 != -1)
	{
		QString hexvalue = QString("#%1").arg(text.color24, 6, 16, QLatin1Char('0'));
		drawColor = QColor(hexvalue);
	}

	QFont font;
	font.setPixelSize(int(text.fontSize * SCALE));
	font.setBold(false);
	font.setFamily("Calibri");


	QGraphicsTextItem* io = new QGraphicsTextItem;
	io->setPos(text.loc1.x * SCALE, text.loc1.y * SCALE);
	io->setPlainText(text.text);
	io->setFont(font);
	io->setDefaultTextColor(drawColor);

	dxf_scene->addItem(io);
}

void MainWindow::drawCircle(DXFCircle const& circle)
{
	QColor drawColor(Qt::black);
	if(circle.color24 != -1)
	{
		QString hexvalue = QString("#%1").arg(circle.color24, 6, 16, QLatin1Char('0'));
		drawColor = QColor(hexvalue);
	}
	const QPen mainPen(drawColor, 1);
	const QBrush blackbrush(drawColor, Qt::SolidPattern);
	dxf_scene->addEllipse(circle.loc.x * SCALE, circle.loc.y * SCALE, circle.radius * SCALE * 4, circle.radius * SCALE * 4, mainPen, blackbrush);
}

void MainWindow::drawLine(DXFLine const& /*line*/)
{
	/*
	QColor drawColor(Qt::black);
	if(line.color24 != -1)
	{
		QString hexvalue = QString("#%1").arg(line.color24, 6, 16, QLatin1Char('0'));
		drawColor = QColor(hexvalue);
	}
	const QPen mainPen(drawColor, 1);
	const QBrush blackbrush(drawColor, Qt::SolidPattern);

	if (line.color == 252) {
		mainPen.setStyle(Qt::DotLine);
	}
	//QBrush blackbrush(Qt::black,Qt::NoBrush);
	dxf_scene->addLine(line.loc1.x  * SCALE, line.loc1.y  * SCALE, line.loc2.x  * SCALE, line.loc2.y * SCALE, mainPen);
	*/
}

void MainWindow::ProcessCommandLine()
{
	QCommandLineParser parser;
	parser.setApplicationDescription("xDXF Viewer");

	QCommandLineOption fileOption(QStringList() << "f" << "file",
		"File to Load.",
		"file");
	parser.addOption(fileOption);

	helpText = parser.helpText();

	parser.parse(QCoreApplication::arguments());

	auto lastDxf{ settings->value("last_dxf").toString() };


	if (!parser.value(fileOption).isEmpty() && QFile::exists(parser.value(fileOption)))
	{
		OpenDXF(parser.value(fileOption));
	}
	else if (QFile::exists(lastDxf))
	{
		OpenDXF(lastDxf);
	}
}