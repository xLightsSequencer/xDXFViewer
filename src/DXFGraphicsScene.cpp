#include "DXFGraphicsScene.h"

#include <QGraphicsSceneMouseEvent>

#include <QtGui>

DXFGraphicsScene::DXFGraphicsScene(QObject* parent) :
	QGraphicsScene(parent) { }

void DXFGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
	// -y as screen 0,0 is top left
	emit mouseDoubleClick(mouseEvent->scenePos().x(), -mouseEvent->scenePos().y());
}

void DXFGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
	// -y as screen 0,0 is top left
	emit mousePosition(mouseEvent->scenePos().x(), -mouseEvent->scenePos().y());
}