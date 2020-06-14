#include "graphic_branch.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QStyle>
#include <cmath>
#include <QDebug>

namespace Ohmcha
{

GraphicBranch::GraphicBranch()
    : GraphicComponent()
{
    component = new Branch;
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

void GraphicBranch::setFirstAnchor(GraphicComponent *item, const QPointF &point)
{
    first = item;
    pFirst = point;
}

void GraphicBranch::setSecondAnchor(GraphicComponent *item, const QPointF &point)
{
    second = item;
    pSecond = point;
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

}
