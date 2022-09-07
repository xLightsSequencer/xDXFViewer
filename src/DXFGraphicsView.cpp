#include "DXFGraphicsView.h"

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QMouseEvent>
#include <QWheelEvent>


DXFGraphicsView::DXFGraphicsView(QWidget* parent) : QGraphicsView(parent) {

	setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	setDragMode(ScrollHandDrag);
}

//stole from stack overflow
void DXFGraphicsView::wheelEvent(QWheelEvent* event) {

	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

	// Scale the view / do the zoom
	double scaleFactor = 1.15;
	if (event->delta() > 0) {
		// Zoom in
		scale(scaleFactor, scaleFactor);
	}
	else {
		// Zooming out
		scale(1.0 / scaleFactor, 1.0 / scaleFactor);
	}
}

void DXFGraphicsView::mousePressEvent(QMouseEvent* e)
{
	QGraphicsView::mousePressEvent(e);
}

void DXFGraphicsView::mouseReleaseEvent(QMouseEvent* e)
{
	QGraphicsView::mouseReleaseEvent(e);
}

