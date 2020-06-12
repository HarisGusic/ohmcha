#include "circuitview.h"
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QScrollBar>
#include <cmath>
#include <QGraphicsItem>
#include <QDebug>
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
    setDragMode(QGraphicsView::RubberBandDrag);
    setMouseTracking(true);
    setGridVisibility(true);
}

void CircuitView::insertComponent(GraphicComponent *component, CircuitView::Mode insertMode)
{
    mode = insertMode;
    if (insertMode != Mode::InsertPoints)
        return; // This will be implemented later
    //TODO Calculate node count
    static int state = 0;
    int *state_ptr = &state;
    pendingInsert = component;
    mouseCallback = [](QMouseEvent *event, CircuitView *owner) {
        if (event->button() != Qt::LeftButton || event->type() != QEvent::MouseButtonRelease)
            return;
        state++;
        QPointF cursorPos = owner->getCursorPosition();
        owner->pendingInsert->setCenter(cursorPos);
        owner->scene()->addItem(owner->pendingInsert);
        owner->pendingInsert = nullptr;
        emit owner->componentInserted();
        owner->mode = CircuitView::Idle;
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
    // _dragPos is used both when dragging the scene and when making a rubber band selection.
    _dragPos = event->pos();
    if (event->button() == Qt::MiddleButton)
    {
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

    _selectionModeDetermined = false;

    QGraphicsView::mouseReleaseEvent(event);
}

void CircuitView::mouseMoveEvent(QMouseEvent *event)
{

    // Store the raw cursor position, useful for snapping to grid
    rawCursorPos = mapToScene(event->pos());

    if (snapOn) snapToGrid();
    else cursorPos = rawCursorPos;

    // Perform scene drag
    if (_dragging)
    {
        QRectF sRect = sceneRect();
        sRect.translate((_dragPos - event->pos()) / zoomLevel);
        setSceneRect(sRect);
        _dragPos = event->pos();
    }
    else
        updateCursorGuides();

    // AutoCAD-like selection behavior
    if (event->buttons() == Qt::LeftButton && !_selectionModeDetermined)
    {
        auto xDiff = event->pos().x() - _dragPos.x();
        _selectionModeDetermined = xDiff != 0;
        if (xDiff > 0)
        { // Dragged to the right: items will be selected if they are wholly contained in the rubber band.
            setRubberBandSelectionMode(Qt::ContainsItemShape);
            setStyleSheet("selection-background-color: normal");
        }
        else
        { // Dragged to the left: items will be selected if they intersect the rubber band.
            setRubberBandSelectionMode(Qt::IntersectsItemShape);
            setStyleSheet("selection-background-color: green");
        }

    }
    qDebug() << rubberBandSelectionMode();

    QGraphicsView::mouseMoveEvent(event);
}

void CircuitView::resizeEvent(QResizeEvent *event)
{
    //TODO: disabled this, because it caused the window to freeze. Find a fix.
    //setSceneRect(getViewRect(this));
    updateCursorGuides();
}

void CircuitView::zoomIn(float scale)
{
    this->zoomLevel *= scale;
    this->scale(scale, scale);

    // Keep the guides scale-independent
    auto pen = hGuide->pen();
    pen.setWidthF(0.5 / zoomLevel);
    hGuide->setPen(pen);
    vGuide->setPen(pen);
}

void CircuitView::resetZoom()
{
    this->resetTransform();
    zoomLevel = 1;

    // Keep the guides scale-independent
    auto pen = hGuide->pen();
    pen.setWidthF(0.5 / zoomLevel);
    hGuide->setPen(pen);
    vGuide->setPen(pen);
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
    if (!hGuide)
    { // If one guide is nullptr, the other one must be as well
        QPen guidePen(QBrush(), 0.5 / zoomLevel);
        guidePen.setColor({0, 0, 0, 200});
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
