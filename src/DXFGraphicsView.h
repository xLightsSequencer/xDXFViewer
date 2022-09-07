#ifndef DXF_GRAPHICS_VIEW_H
#define DXF_GRAPHICS_VIEW_H
 
#include <QGraphicsView>
#include <QGraphicsRectItem>
 
class DXFGraphicsView : public QGraphicsView
{
	Q_OBJECT
public:
    DXFGraphicsView(QWidget* parent = nullptr);

Q_SIGNALS:
	void MouseSelectSignal(const QPoint&);

protected:
	void mousePressEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;
	virtual void wheelEvent(QWheelEvent* e) override;

public Q_SLOTS:

};
#endif // DXF_GRAPHICS_VIEW_H