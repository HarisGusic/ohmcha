#include "circuitview.h"
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QScrollBar>
#include <cmath>
#include <QGraphicsItem>

CircuitView::CircuitView(QWidget *parent)
    : QGraphicsView(new QGraphicsScene, parent)
{
}

void CircuitView::drawLine(float x1, float y1, float x2, float y2)
{
    scene()->addLine(x1, y1, x2, y2);
}

void CircuitView::insertComponent(Component *component, CircuitView::Mode insertMode)
{
    mode = insertMode;
    if (insertMode != Mode::InsertPoints)
        return; // This will be implemented later
    //TODO Calculate node count
    static int state = 0;
    int *state_ptr = &state;
    mouseCallback = [](QMouseEvent *event, CircuitView *owner) {
        if (event->button() != Qt::LeftButton || event->type() != QEvent::MouseButtonRelease)
            return;
        state++;
        QPointF cursorPos = owner->mapToScene(owner->mapFromGlobal(QCursor::pos()));
        auto ellipse = owner->scene()->addEllipse(0, 0, 3, 3);
        ellipse->setTransform(QTransform::fromTranslate(cursorPos.x(), cursorPos.y()));
    };
    if (state == 2) // Replace with component.nodeCount
    {

    }
}

void CircuitView::wheelEvent(QWheelEvent *event)
{
    float scale = std::min(std::max(event->angleDelta().y() / 60.f, 1 / 1.2f), 1.2f);
    zoomIn(scale);
}

void CircuitView::mousePressEvent(QMouseEvent *event)
{
    if (mode != Idle)
    {
        if (mouseCallback)
            mouseCallback(event, this);
        return;
    }
    setDragMode(QGraphicsView::ScrollHandDrag);
    QGraphicsView::mousePressEvent(event);
}

void CircuitView::mouseReleaseEvent(QMouseEvent *event)
{
    if (mode != Idle)
    {
        if (mouseCallback)
            mouseCallback(event, this);
        return;
    }
    setDragMode(QGraphicsView::NoDrag);
    QGraphicsView::mouseReleaseEvent(event);
}

void CircuitView::zoomIn(float scale)
{
    this->zoomLevel = scale;
    this->scale(scale, scale);
}

void CircuitView::resetZoom()
{
    this->resetTransform();
}
