#ifndef DXF_GRAPHIC_SCENE_H
#define DXF_GRAPHIC_SCENE_H

#include <QGraphicsScene>
#include <QObject>

class DXFGraphicsScene : public QGraphicsScene
{
	Q_OBJECT
public:
	DXFGraphicsScene(QObject* parent = nullptr);

public Q_SLOTS:
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* mouseEvent) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent) override;


Q_SIGNALS:
	void mousePosition(qreal, qreal);
	void mouseDoubleClick(qreal, qreal);
};

#endif // DXF_GRAPHIC_SCENE_H