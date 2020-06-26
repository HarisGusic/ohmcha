#include "circuitview.h"
#include <QGraphicsScene>
#include <QWheelEvent>
#include <QScrollBar>
#include <cmath>
#include <QGraphicsItem>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "graphic_component.h"
#include "graphic_node.h"
#include "src/model/kirchhoff.h"
#include <QtMath>

namespace Ohmcha
{

/*********
 * Scene *
 *********/

CircuitViewScene::CircuitViewScene(CircuitView *circuitView)
    : QGraphicsScene(), circuitView(circuitView)
{
    connect(this, &QGraphicsScene::selectionChanged, this, &CircuitViewScene::on_selectionChanged);
}

void CircuitViewScene::clear()
{
    for (auto *c : items())
    {
        if (dynamic_cast<GraphicBranch*>(c))
        {
            removeItem(c);
            delete c;
        }
    }
    for (auto *c : items())
    {
        removeItem(c);
        delete c;
    }
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
    QGraphicsScene::keyPressEvent(event);
    if (event->key() == Qt::Key::Key_Escape && _insertedBranch != nullptr)
    {
        removeItem(_insertedBranch);
        delete _insertedBranch;
        _insertedBranch = nullptr;
        circuitView->setDragMode(QGraphicsView::RubberBandDrag);
    }
    if (event->key() == Qt::Key::Key_Delete)
    {
        for (auto x : circuitView->results)
        {
            for (auto item : selectedItems())
                if (item == x->parentItem())
                    x->setParentItem(nullptr);
        }
        // First delete all branches
        foreach(auto *item, selectedItems())
        {
            if (dynamic_cast<GraphicBranch*>(item))
            {
                emit circuitView->itemDeleted((GraphicComponent*) item);
                removeItem(item);
                delete item;
            }
        }

        // Delete the rest
        foreach(auto *item, selectedItems())
        {
            // Delete all branches connected to this node
            for (auto *branch : items())
            {
                if (dynamic_cast<GraphicBranch*>(branch) && ((GraphicBranch*) branch)->isConnectedTo((GraphicComponent*) item))
                {
                    emit circuitView->itemDeleted((GraphicComponent*) branch);
                    removeItem(branch);
                    delete branch;
                }
            }
            auto &components = circuitView->schematic->getComponents();
            // Remove the corresponding component from the schematic
            for (auto it = components.begin(); it != components.end(); ++it)
            {
                if (*it == ((GraphicComponent*) item)->getComponent())
                {
                    it = components.erase(it);
                    --it;
                }
            }
            emit circuitView->itemDeleted((GraphicComponent*) item);
            removeItem(item);
            delete item;
        }
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

void CircuitViewScene::on_selectionChanged()
{
    update();
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

QPointF nearest(GraphicComponent *c, QPointF point)
{
    auto terminals = c->getTerminals();
    float sqDist = 1e20;
    QPointF result;
    for (auto term : terminals)
    {
        float tmp = QPointF::dotProduct(term - point, term - point);
        if (tmp < sqDist)
        {
            result = term;
            sqDist = tmp;
        }
    }
    return result;
}

QPointF getOtherTerminal(GraphicComponent *next, const QPointF &pNext)
{
    return next->getTerminals()[1 - next->getTerminalId(pNext)];
}

GraphicComponent *findComponent(Component *component, QGraphicsScene *scene)
{
    for (auto *item : scene->items())
    {
        if (!dynamic_cast<GraphicComponent*>(item))
            continue;
        auto gc = (GraphicComponent*) item;

        if (gc->getComponent() == component)
            return gc;
    }
    return nullptr;
}

GraphicBranch *findBranch(Component *component, QGraphicsScene *scene)
{
    for (auto *item : scene->items())
    {
        if (!dynamic_cast<GraphicBranch*>(item))
            continue;
        auto gc = (GraphicBranch*) item;

            if (gc->getComponent() == component && (dynamic_cast<GraphicNode*>(gc->getFirstAnchor())
                    || dynamic_cast<GraphicNode*>(gc->getSecondAnchor())))
                return gc;
    }
    return nullptr;
}

CircuitView::CircuitView(QWidget *parent, Schematic *schematic)
    : CircuitView(parent)
{
    this->schematic = schematic;
    for (Component *c : schematic->getComponents())
    {
        auto *item = GraphicComponent::newFromComponent(c);
        scene()->addItem(item);
    }
    for (Node *n : schematic->getNodes())
        scene()->addItem(new GraphicNode(n));
    for (Branch *b : schematic->getBranches())
    {
        // If branch is empty or is hanging, skip it
        if (b->attached.empty() || !b->getNode1() || !b->getNode2())
            continue;

        QList<GraphicComponent*> connectees;
        // Find graphic components and nodes connected to this branch
        for (auto *c : b->attached)
            connectees.push_back(findComponent(c, scene()));
        /*for (auto *c : scene()->items())
        {
            // Ignore branches and non-components
            if (!dynamic_cast<GraphicComponent*>(c) || dynamic_cast<GraphicBranch*>(c))
                continue;

            auto component = (GraphicComponent*) c;
            if (std::find(b->attached.begin(), b->attached.end(), component->getComponent()) != b->attached.end())
                connectees.push_back(component);
            else if (dynamic_cast<GraphicNode*>(c))
                nodes.push_back((GraphicNode*) component);

        }*/
        connectees.push_front(findComponent(b->getNode1(), scene()));
        connectees.push_back(findComponent(b->getNode2(), scene()));

        GraphicBranch *branch = nullptr;
        QPointF lastTerm;
        for (int i = 0; i < connectees.size() - 1; ++i)
        {
            GraphicBranch *branch = new GraphicBranch;
            auto *conn = connectees[i];

            if (dynamic_cast<GraphicNode*>(conn))
                branch->setFirstAnchor(conn, conn->getTerminals()[0]);
            else
                branch->setFirstAnchor(conn, conn->getTerminals()[b->inversions[i-1] ? 0 : 1]);
            conn = connectees[i+1];
            if (dynamic_cast<GraphicNode*>(conn))
                branch->setSecondAnchor(conn, conn->getTerminals()[0]);
            else
                branch->setSecondAnchor(conn, conn->getTerminals()[b->inversions[i] ? 1 : 0]);
            scene()->addItem(branch);
        }
    }
}

CircuitView::~CircuitView()
{
    scene()->clear();
    delete schematic;
    //TODO delete schematic
}

void CircuitView::initialize()
{
    setSceneRect(getViewRect(this));
    updateCursorGuides();
}

void CircuitView::initiateInsertComponent(GraphicComponent *component, CircuitView::Mode insertMode)
{
    // TODO insertMode is not currently used.
    mode = insertMode;
    pendingInsert = component;
    component->setVisible(false);
    scene()->addItem(component);
}

void CircuitView::solve()
{
    getSchematic();
    auto sol = khoffSolve(schematic);
    for (auto item : results)
    {
        scene()->removeItem(item);
        delete item;
    }
    results.clear();
    // Displays for nodes
    for (int i = 0; i < schematic->getNodes().size(); ++i)
    {
        auto node = schematic->getNodes()[i];
        auto gc = findComponent(node, scene());

        float value = 0;
        if (node != schematic->getNodes()[schematic->getNodes().size() - 1]) // Not last node
            value = sol(i);

        QGraphicsTextItem *text = new QGraphicsTextItem(QString::number(value));
        text->setPos(QPointF{0, -10});
        text->setDefaultTextColor(QColor(30, 240, 56));
        text->setZValue(200);
        QGraphicsRectItem *rect = new QGraphicsRectItem(QRectF(0, 0, 50, 15));
        rect->setPos({0, -5});
        rect->setBrush(QBrush(QColor(255, 255, 255)));
        rect->setPen(QPen(QColor(255, 255, 255)));
        rect->setZValue(199);

        results.push_back(rect);
        results.push_back(text);
        scene()->addItem(rect);
        scene()->addItem(text);
        rect->setParentItem(gc);
        text->setParentItem(gc);
    }

    // Displays for branches
    for (int i = 0; i < schematic->getBranches().size(); ++i)
    {
        auto branch = schematic->getBranches()[i];
        auto gc = findBranch(branch, scene());

        float value = sol(i + schematic->getNodes().size() - 1);

        QPainterPath path;
        path.moveTo(0, 0);
        path.lineTo(value > 0 ? -6 : 6, -4);
        path.moveTo(0, 0);
        path.lineTo(value > 0 ? -6 : 6, 4);
        auto pathItem = scene()->addPath(path);

        value = qAbs(value);

        pathItem->setZValue(199);
        QGraphicsTextItem *text = new QGraphicsTextItem(QString::number(value));
        text->setDefaultTextColor(QColor(30, 240, 56));
        text->setZValue(200);
        QGraphicsRectItem *rect = new QGraphicsRectItem(QRectF(0, 0, 50, 15));
        rect->setBrush(QBrush(QColor(255, 255, 255)));
        rect->setPen(QPen(QColor(255, 255, 255)));
        rect->setZValue(198);

        results.push_back(rect);
        results.push_back(text);
        rect->setParentItem(gc);
        text->setParentItem(gc);
        auto pos = (
                 gc->getFirstAnchor()->mapToItem(gc, gc->getFirstAnchorPoint())
                 + gc->getSecondAnchor()->mapToItem(gc, gc->getSecondAnchorPoint())) / 2;
        auto scenePos = gc->mapToScene(pos);
        text->setPos(pos + QPointF(0, -10));
        rect->setPos(text->pos() + QPointF(0, 5));

        auto deltaPos =
                 gc->getFirstAnchor()->mapToScene(gc->getFirstAnchorPoint()) -
                 gc->getSecondAnchor()->mapToScene(gc->getSecondAnchorPoint());
        float angle = qAtan2(deltaPos.y(), deltaPos.x());
        GraphicNode *node;
        bool mult = false;
        if (dynamic_cast<GraphicNode*>(gc->getFirstAnchor()))
            node = (GraphicNode*) gc->getFirstAnchor();
        else if (dynamic_cast<GraphicNode*>(gc->getSecondAnchor()))
        {
            node = (GraphicNode*) gc->getSecondAnchor();
            mult = true;
        }
        Branch *br = (Branch*) gc->getComponent();
        if (node->getComponent() == br->getNode2())
            mult = !mult;
        if (mult)
            angle += 3.1415926;

        pathItem->setPos(pos);
        pathItem->setRotation(angle * 180 / 3.1415926);

        results.push_back(pathItem);
        pathItem->setParentItem(gc);
    }
}

void CircuitView::showValues(bool show)
{
    if (show)
        for (auto item : results)
            item->show();
    else
        for (auto item : results)
            item->hide();
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

GraphicBranch *findBranch(GraphicNode *node, Schematic *schematic, QGraphicsScene *scene)
{
    for (auto *b : scene->items())
    {
        if (!dynamic_cast<GraphicBranch*>(b))
            continue;
        if (!((GraphicBranch*) b)->isConnectedTo(node))
            continue;
        if (std::find(schematic->getBranches().begin(), schematic->getBranches().end(), (Branch*) ((GraphicBranch*) b)->getComponent()) != schematic->getBranches().end())
            continue;
        //if (dynamic_cast<GraphicBranch*>(b) && ((GraphicBranch*) b)->isConnectedTo(node) &&
        //    std::find(schematic->getBranches().begin(), schematic->getBranches().end(), (Branch*) ((GraphicBranch*) b)->getComponent()) == schematic->getBranches().end())
            return (GraphicBranch*) b;
    }
    return nullptr;
}

void nextElement(GraphicBranch *br, GraphicComponent *&next, QPointF *pNext)
{
    if (br->getFirstAnchor() != next)
    {
        next = br->getFirstAnchor();
        *pNext = br->getFirstAnchorPoint();
    }
    else if (br->getSecondAnchor() != next)
    {
        next = br->getSecondAnchor();
        *pNext = br->getSecondAnchorPoint();
    }
    else
        next = nullptr;
}

void insertToBranch(GraphicComponent *next, const QPointF &pNext, Branch *_branch)
{
    int terminalId = next->getTerminalId(pNext);
    _branch->addComponent(next->getComponent(), terminalId != 0);
    _branch->attached.push_back(next->getComponent());
    _branch->inversions.push_back(terminalId != 0);
}

GraphicBranch *getOtherBranch(GraphicComponent *next, GraphicComponent *thisBranch, QGraphicsScene *scene)
{
    for (auto *c : scene->items())
        if (dynamic_cast<GraphicBranch*>(c) && ((GraphicBranch*) c)->isConnectedTo(next) && c != thisBranch)
            return (GraphicBranch*) c;
    return nullptr;
}

Schematic *CircuitView::getSchematic()
{
    if (schematic == nullptr)
    {
        schematic = new Schematic;
        for (auto *c : scene()->items())
        {
            if (!dynamic_cast<GraphicComponent*>(c) || dynamic_cast<GraphicNode*>(c) || dynamic_cast<GraphicBranch*>(c))
                continue;
            schematic->add(((GraphicComponent*) c)->getComponent());
        }
    }
    schematic->clearBranches(); //TODO delete?
    schematic->clearNodes();

    for (auto *c : scene()->items())
    {
        if (dynamic_cast<GraphicNode*>(c)) // Found a node
        {
            auto node = (GraphicNode*) c;
            while (true)
            {
                // Find a branch that is connected to this node
                GraphicBranch *branch = findBranch(node, schematic, scene());
                if (branch == nullptr)
                    break;

                branch->setComponent(new Branch);

                Branch *_branch = (Branch*) branch->getComponent();
                _branch->setNode1(*(Node*) node->getComponent());

                // Follow down the branch
                GraphicComponent *next = node;
                QPointF pNext;
                for (GraphicBranch *br = branch; ;)
                {
                    br->setComponent(_branch);
                    // Find the next element this branch is connected to, store it into next
                    nextElement(br, next, &pNext);

                    if (dynamic_cast<GraphicNode*>(next))
                    {
                        _branch->setNode2(*(Node*) next->getComponent());
                        break;
                    }

                    // Insert the next component into the model branch
                    insertToBranch(next, pNext, _branch); //TODO not properly implemented
                    br = getOtherBranch(next, br, scene());
                    if (br == nullptr) break;
                }
                schematic->add(_branch);
            }
            node->getComponent()->setPosition(new Component::Pos{(float) node->pos().x(), (float) node->pos().y()});
            schematic->add(node->getComponent());
        }
    }
    return schematic;
}

void CircuitView::wheelEvent(QWheelEvent *event)
{
    float scale = std::min(std::max(event->angleDelta().y() / 60.f, 1 / 1.2f), 1.2f);
    zoomIn(scale);
}

void CircuitView::mousePressEvent(QMouseEvent *event)
{
    showValues(false);
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
    if (mode != Idle && event->button() != Qt::MiddleButton)
    {
        pendingInsert->setFlag(QGraphicsItem::ItemIsMovable);
        pendingInsert->setFlag(QGraphicsItem::ItemIsSelectable);
        mode = Idle;
        if (schematic)
            schematic->add(pendingInsert->getComponent());
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

    if (pendingInsert)
        setFocus();

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

void CircuitView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape && pendingInsert)
    {
        scene()->removeItem(pendingInsert);
        delete pendingInsert;
        pendingInsert = nullptr;
        mode = Idle;
    }
    QGraphicsView::keyPressEvent(event);
}

void CircuitView::resizeEvent(QResizeEvent *event)
{
    //TODO: disabled this, because it caused the window to freeze. Find a fix.
    //setSceneRect(getViewRect(this));
    updateCursorGuides();
    scene()->update();
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
    scene()->update();
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
