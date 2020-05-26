#include "circuitview.h"
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QScrollBar>
#include <cmath>
#include <QGraphicsItem>
#include "graphic_component.h"

namespace Ohmcha
{
// Helper function: get the extents of the scene
QRectF getViewRect(QGraphicsView *view)
{
    QPointF A = view->mapToScene(0, 0);
    QPointF B = view->mapToScene(view->viewport()->width(), view->viewport()->height());
    return QRectF(A, B);
}

CircuitView::CircuitView(QWidget *parent)
    : QGraphicsView(new QGraphicsScene, parent)
{
    setSceneRect(getViewRect(this));
    setCursor(Qt::BlankCursor);
    setMouseTracking(true);
    setGridVisibility(true);
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
        QPointF cursorPos = owner->getCursorPosition();
        GraphicResistor *r = new GraphicResistor(cursorPos, cursorPos);
        owner->scene()->addItem(r);
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
    if (event->button() == Qt::MiddleButton)
    {
        _dragPos = event->pos();
        _dragging = true;
        setCursor(Qt::ClosedHandCursor);
        hGuide->setVisible(false);
        vGuide->setVisible(false);
    }
    if (mode != Idle)
    {
        if (mouseCallback)
            mouseCallback(event, this);
        return;
    }
    QGraphicsView::mousePressEvent(event);
}

void CircuitView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton)
    {
        _dragging = false;
        setCursor(Qt::BlankCursor);
        updateCursorGuides();
        hGuide->setVisible(true);
        vGuide->setVisible(true);
    }
    if (mode != Idle)
    {
        if (mouseCallback)
            mouseCallback(event, this);
        return;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void CircuitView::mouseMoveEvent(QMouseEvent *event)
{
    rawCursorPos = mapToScene(event->pos());

    if (snapOn) snapToGrid();
    else cursorPos = rawCursorPos;


    // Perform drag
    if (_dragging)
    {
        QRectF sRect = sceneRect();
        sRect.translate(event->pos() - _dragPos);
        setSceneRect(sRect);
        _dragPos = event->pos();
    }
    else
        updateCursorGuides();
}

void CircuitView::resizeEvent(QResizeEvent *event)
{
    setSceneRect(getViewRect(this));
    updateCursorGuides();
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

void CircuitView::setGridVisibility(bool visibility)
{
    showGrid = visibility;
    if (showGrid)
    {
        //TODO change the scaling behavior
        //scene()->setBackgroundBrush(Qt::CrossPattern);
    }
    else
        scene()->setBackgroundBrush(Qt::NoBrush);
}

void CircuitView::setSnap(bool state)
{
    snapOn = state;
    snapToGrid();
}

QPointF CircuitView::getCursorPosition()
{
    return cursorPos;
}

void CircuitView::updateCursorGuides()
{
    QRectF rect = getViewRect(this);
    QPen guidePen(QBrush(), 0.5);
    guidePen.setColor({0, 0, 0, 200});
    if (!hGuide)
    { // If one guide is nullptr, the other one must be as well
        hGuide = scene()->addLine(rect.left(), cursorPos.y(), rect.right(), cursorPos.y(), guidePen);
        vGuide = scene()->addLine(cursorPos.x(), rect.top(), cursorPos.x(), rect.bottom(), guidePen);
    }
    else
    {
        hGuide->setLine(rect.left(), cursorPos.y(), rect.right(), cursorPos.y());
        vGuide->setLine(cursorPos.x(), rect.top(), cursorPos.x(), rect.bottom());
    }
    // Update instantly to reduce input lag
    hGuide->update();
    vGuide->update();
    viewport()->repaint();
}

void CircuitView::snapToGrid()
{
    // Cursor position relative to the center of the entire scene rect
    auto _cursorPos = rawCursorPos - scene()->sceneRect().center();
    int nDeltaX = _cursorPos.x() / gridSpacingX,
            nDeltaY = _cursorPos.y() / gridSpacingY;
    cursorPos = scene()->sceneRect().center() + QPointF(nDeltaX * gridSpacingX, nDeltaY * gridSpacingY);
}

}
