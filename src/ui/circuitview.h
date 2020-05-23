#ifndef CIRCUITVIEW_H
#define CIRCUITVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsItem>

class Component;

class CircuitView : public QGraphicsView
{
    Q_OBJECT

    float zoomLevel = 1;
public:
    enum Mode {Idle, InsertDragDrop, InsertPoints};
    Mode mode = Idle;
    void *editBuffer = nullptr;

public:
    // Constructors
    CircuitView(QWidget *parent = nullptr);

    // Methods
    void drawLine(float x1, float y1, float x2, float y2);
    void insertComponent(Component *component, Mode insertMode);

    void zoomIn(float scale = 1.2);
    void resetZoom();

protected:

    void  (*mouseCallback)(QMouseEvent *event, CircuitView *owner);

    // Event handling
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

};

#endif // CIRCUITVIEW_H
