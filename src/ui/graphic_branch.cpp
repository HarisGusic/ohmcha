#include "graphic_branch.h"
#include "graphic_node.h"

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
    component = new Branch;
    setZValue(0);
    setFlag(ItemIsMovable, false);
    setFlag(ItemIsSelectable, false);
}

GraphicBranch::GraphicBranch(Branch *branch)
    : GraphicBranch()
{
    component = branch;
}

void GraphicBranch::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    // Mouse is hovering over the item
    if (option->state & QStyle::State_MouseOver && second != nullptr)
    {
        auto pen = painter->pen();
        pen.setColor({0x8c, 0x9e, 0xff});
        painter->setPen(pen);
    }
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

    GraphicBranch *newBranch = new GraphicBranch((Branch*) component);

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
}

void GraphicBranch::setSecondAnchor(GraphicComponent *item, const QPointF &point)
{
    second = item;
    pSecond = point;
    if (item != nullptr)
        setZValue(50);
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

void GraphicBranch::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    split(event->pos());
}

}
