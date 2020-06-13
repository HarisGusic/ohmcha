#include "graphic_branch.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QStyle>
#include <cmath>

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
    if (option->state & QStyle::State_MouseOver)
    {
        auto pen = painter->pen();
        pen.setColor({0x8c, 0x9e, 0xff});
        painter->setPen(pen);
    }
    painter->drawLine(first->mapToItem(this, pFirst), first->mapToItem(this, pSecond));
}

QRectF GraphicBranch::boundingRect() const
{
    return QRectF(std::min(pFirst.x(), pSecond.x()) - pen.width() / 2, std::min(pFirst.y(), pSecond.x()) - pen.width() / 2,
                  std::max(pFirst.x(), pSecond.x()) - pen.width() / 2, std::max(pFirst.y(), pSecond.x()) - pen.width() / 2);
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
