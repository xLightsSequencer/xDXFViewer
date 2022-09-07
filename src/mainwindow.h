#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "spdlog/spdlog.h"
#include "spdlog/common.h"

#include <memory>
#include <filesystem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }

class QListWidgetItem;
class QListWidget;
class QTableWidget;
class QSettings;
QT_END_NAMESPACE

class DXFGraphicsScene;
struct DXFText;
struct DXFCircle;
struct DXFLine;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

public Q_SLOTS:

	void on_actionOpen_DXF_triggered();

	void on_actionPrint_triggered();

	void on_actionClose_triggered();

	void on_actionAbout_triggered();
	void on_actionOpen_Logs_triggered();

	void ProcessCommandLine();

private:
	Ui::MainWindow* ui;
	std::shared_ptr<spdlog::logger> logger{ nullptr };
	std::unique_ptr<QSettings> settings{ nullptr };

	std::unique_ptr<DXFGraphicsScene> dxf_scene{ nullptr };

	QString appdir;
	QString helpText;

	void OpenDXF(QString const& file);
	void drawText(DXFText const& text);
	void drawCircle(DXFCircle const& circle);
	void drawLine(DXFLine const& line);

};
#endif // MAINWINDOW_H
