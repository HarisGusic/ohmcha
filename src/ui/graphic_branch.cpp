#include "graphic_branch.h"
#include "graphic_node.h"
#include "circuitview.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QStyle>
#include <cmath>
#include <QDebug>
#include <QtMath>

namespace Ohmcha
{

GraphicBranch::GraphicBranch()
    : GraphicComponent()
{
    setZValue(0);
    setFlag(ItemIsMovable, false);
}

GraphicBranch::GraphicBranch(Branch *branch)
    : GraphicBranch()
{
    component = branch;
    synchronize();
}

GraphicBranch::GraphicBranch(const GraphicBranch &original)
    : GraphicComponent(original)
{

}

GraphicBranch::~GraphicBranch()
{
    if (first && first->getTerminalId(pFirst) == 0)
        first->branch1 = false;
    if (second && second->getTerminalId(pSecond) == 0)
        second->branch1 = false;
    if (first && first->getTerminalId(pFirst) == 1)
        first->branch2 = false;
    if (second && second->getTerminalId(pSecond) == 1)
        second->branch2 = false;
}

void GraphicBranch::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    applyColors(painter, option);

    if (first != nullptr)
    {
        QPointF b;
        if (second == nullptr) // The second anchor is the scene
            b = mapFromScene(pSecond);
        else
            b = mapFromItem(second, pSecond);
        painter->drawLine(mapFromItem(first, pFirst), b);
    }
}

QRectF GraphicBranch::boundingRect() const
{
    QPointF a = mapFromItem(first, pFirst),
            b = second == nullptr ? mapFromScene(pSecond) : mapFromItem(second, pSecond);
    return QRectF(
                QPointF(std::min(a.x(), b.x()) - pen.width() / 2, std::min(a.y(), b.y()) - pen.width() / 2),
                QPointF(std::max(a.x(), b.x()) + pen.width() / 2, std::max(a.y(), b.y()) + pen.width() / 2)
                );
}

QPainterPath GraphicBranch::shape() const
{
    QPointF a = mapFromItem(first, pFirst),
            b = mapFromItem(second, pSecond);

    QPointF vec = a - b;

    vec = vec * (3.0 / qSqrt(QPointF::dotProduct(vec, vec)));
    QPointF orthogonal(vec.y(), -vec.x());

    QPainterPath result(a + orthogonal);
    result.lineTo(a - orthogonal);
    result.lineTo(b - orthogonal);
    result.lineTo(b + orthogonal);
    result.closeSubpath();

    return result;
}

void GraphicBranch::swapTerminals()
{
    auto tmp = first;
    auto pTmp = pFirst;

    first = second;
    pFirst = pSecond;
    second = tmp;
    pSecond = pTmp;
}

GraphicComponent *GraphicBranch::copy() const
{
    return new GraphicBranch(*this);
}

void GraphicBranch::split(QPointF point)
{
    // Calculate the projection of the point onto the branch to prevent it from bending
    // Vector connecting the first anchor to 'point'
    QPointF f = mapFromItem(first, pFirst), s = mapFromItem(second, pSecond);
    QPointF fs = s - f, fp = point - f;

    fp = QPointF::dotProduct(fp, fs) / QPointF::dotProduct(fs,fs) * fs;

    point = f + fp;

    // Create the new node and branch
    // This branch connects first and point, the new branch connects point and second

    GraphicNode *node = new GraphicNode;
    node->setPos(mapToScene(point));

    //TODO temporarily replaced: GraphicBranch *newBranch = new GraphicBranch((Branch*) component);
    GraphicBranch *newBranch = new GraphicBranch();

    newBranch->setFirstAnchor(node, {});
    newBranch->setSecondAnchor(second, pSecond);

    setSecondAnchor(node, {});

    scene()->addItem(node);
    scene()->addItem(newBranch);
}

void GraphicBranch::setFirstAnchor(GraphicComponent *item, const QPointF &point)
{
    first = item;
    pFirst = point;

    int terminalId = item->getTerminalId(point);
    // TODO this does not work for elements with more terminals
    // Flag terminals 1/2 as connected to a branch
    if (terminalId == 0)
        item->branch1 = true;
    else
        item->branch2 = true;
}

void GraphicBranch::setSecondAnchor(GraphicComponent *item, const QPointF &point)
{
    second = item;
    pSecond = point;

    if (item == nullptr)
        return;

    // TODO this does not work for elements with more terminals
    // Flag terminals 1/2 as connected to a branch
    int terminalId = item->getTerminalId(point);
    if (item != nullptr)
        setZValue(50);
    if (terminalId == 0)
        item->branch1 = true;
    else
        item->branch2 = true;
}

GraphicComponent *GraphicBranch::getFirstAnchor() const
{
    return first;
}

QPointF GraphicBranch::getFirstAnchorPoint() const
{
    return pFirst;
}

QPointF GraphicBranch::getSecondAnchorPoint() const
{
    return pSecond;
}

GraphicComponent *GraphicBranch::getSecondAnchor() const
{
    return second;
}

bool GraphicBranch::isConnectedTo(GraphicComponent *item) const
{
    return item == first || item == second;
}

void GraphicBranch::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    auto _scene = dynamic_cast<CircuitViewScene*>(scene());
    if (_scene != nullptr && !_scene->isInsertingComponent())
        split(event->pos());
}

}
