#include "circuitview.h"
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QScrollBar>
#include <cmath>
#include <QGraphicsItem>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "graphic_component.h"

namespace Ohmcha
{

/*********
 * Scene *
 *********/

CircuitViewScene::CircuitViewScene(CircuitView *circuitView)
    : QGraphicsScene(), circuitView(circuitView)
{

}

bool CircuitViewScene::isInsertingComponent() const
{
    return _insertedBranch != nullptr;
}

void CircuitViewScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseMoveEvent(event);
    if (_insertedBranch != nullptr)
    {
        _insertedBranch->setSecondAnchor(nullptr, event->scenePos());
        update();
    }
}

void CircuitViewScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mousePressEvent(event);
}

void CircuitViewScene::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key::Key_Escape && _insertedBranch != nullptr)
    {
        removeItem(_insertedBranch);
        delete _insertedBranch;
        _insertedBranch = nullptr;
        update();
    }
}

void CircuitViewScene::terminalClickEvent(GraphicComponent *source, QPointF terminal)
{
    if (_insertedBranch == nullptr)
    { // The user has clicked on a terminal and begun a branch insertion
        _insertedBranch = new GraphicBranch;
        _insertedBranch->setFirstAnchor(source, terminal);
        _insertedBranch->setSecondAnchor(nullptr, circuitView->getCursorPosition());
        addItem(_insertedBranch);

        // Disable rubber band selection
        circuitView->setDragMode(QGraphicsView::NoDrag);
    }
    else if (_insertedBranch->getFirstAnchor() != source)
    { // Terminal clicked during branch insertion
        _insertedBranch->setSecondAnchor(source, terminal);
        _insertedBranch = nullptr;
        update();

        // Disable rubber band selection
        circuitView->setDragMode(QGraphicsView::RubberBandDrag);
    }
}

/***************
 * CircuitView *
 ***************/

// Helper function: get the extents of the scene
QRectF getViewRect(QGraphicsView *view)
{
    QPointF A = view->mapToScene(0, 0);
    QPointF B = view->mapToScene(view->viewport()->width(), view->viewport()->height());
    return QRectF(A, B);
}

CircuitView::CircuitView(QWidget *parent)
    : QGraphicsView(new CircuitViewScene(this), parent)
{
    setCursor(Qt::BlankCursor);
    setDragMode(QGraphicsView::RubberBandDrag);
    setRenderHint(QPainter::Antialiasing);
    setMouseTracking(true);
    setGridVisibility(true);
}

void CircuitView::initialize()
{
    setSceneRect(getViewRect(this));
    setSceneRect(sceneRect());
}

void CircuitView::initiateInsertComponent(GraphicComponent *component, CircuitView::Mode insertMode)
{
    // TODO insertMode is not currently used.
    mode = insertMode;
    pendingInsert = component;
    component->setVisible(false);
    scene()->addItem(component);
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

CircuitViewScene *CircuitView::scene()
{
    return (CircuitViewScene*) QGraphicsView::scene();
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

void CircuitView::setComponentPreview(ComponentPreview *componentPreview)
{
    this->componentPreview = componentPreview;
}

QPointF CircuitView::getCursorPosition()
{
    return cursorPos;
}

ComponentPreview *CircuitView::getComponentPreview()
{
    return componentPreview;
}

void CircuitView::wheelEvent(QWheelEvent *event)
{
    float scale = std::min(std::max(event->angleDelta().y() / 60.f, 1 / 1.2f), 1.2f);
    zoomIn(scale);
}

void CircuitView::mousePressEvent(QMouseEvent *event)
{
    // Parent method call should be first to prevent selection upon dropping the item in the view.
    if (event->button() != Qt::MiddleButton)
        QGraphicsView::mousePressEvent(event);

    if (event->button() == Qt::MiddleButton)
    {
        _dragging = true;
        setCursor(Qt::ClosedHandCursor);
        hGuide->setVisible(false);
        vGuide->setVisible(false);
    }

    // _dragPos is used both when dragging the scene and when making a rubber band selection.
    _dragPos = event->pos();

}

void CircuitView::mouseReleaseEvent(QMouseEvent *event)
{
    if (!_dragging)
        QGraphicsView::mouseReleaseEvent(event);

    // If a single item is selected, the user wants to edit it in the component preview.
    if (mode == Idle && scene()->selectedItems().size() == 1)
    {
        // NOTE: In a future implementation where items can be annotations (other than components), we must
        // check the actual type of the selected item.
        emit componentSelected((GraphicComponent*) *scene()->selectedItems().begin());
    }

    // A component is being inserted into the CircuitView.
    if (mode != Idle)
    {
        pendingInsert->setFlag(QGraphicsItem::ItemIsMovable);
        pendingInsert->setFlag(QGraphicsItem::ItemIsSelectable);
        mode = Idle;
        pendingInsert = nullptr;
    }

    // Return from scene drag mode to normal mode
    if (event->button() == Qt::MiddleButton)
    {
        _dragging = false;
        setCursor(Qt::BlankCursor);
        updateCursorGuides();
        hGuide->setVisible(true);
        vGuide->setVisible(true);
    }

    _selectionModeDetermined = false;
}

void CircuitView::mouseMoveEvent(QMouseEvent *event)
{
    // TODO: this is a filthy hack so that the view doesn't jump the first time the scene is dragged
    setSceneRect(sceneRect());

    if (!_dragging)
        QGraphicsView::mouseMoveEvent(event);

    // Store the raw cursor position, useful for snapping to grid
    rawCursorPos = mapToScene(event->pos());

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

    // Update component preview position while inserting component.
    if (mode != Idle)
    {
        pendingInsert->setCenter(cursorPos);
        pendingInsert->setVisible(true);
    }

    // Snap works only if an item is being placed or moved
    if (snapOn && (mode != Idle || scene()->mouseGrabberItem() != nullptr))
    {
        snapToGrid();
        event->setLocalPos(mapFromScene(cursorPos));
    }
    else cursorPos = rawCursorPos;
}

void CircuitView::resizeEvent(QResizeEvent *event)
{
    //TODO: disabled this, because it caused the window to freeze. Find a fix.
    //setSceneRect(getViewRect(this));
    updateCursorGuides();
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
