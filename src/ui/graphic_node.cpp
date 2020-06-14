#include "graphic_node.h"

#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPainter>

namespace Ohmcha
{

GraphicNode::GraphicNode()
{
    setZValue(100);
    component = new Node;
}

void GraphicNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Change color if item is hovered over
    if (option->state & QStyle::State_MouseOver)
    {
        QPen pen = painter->pen();
        pen.setColor({0x8c, 0x9e, 0xff});
        painter->setPen(pen);
    }

    // Change color if item is selected
    if (option->state & QStyle::State_Selected)
    {
        auto pen = painter->pen();
        pen.setColor(QColor(0xff,0x8c,0x8c));
        painter->setPen(pen);
    }

    painter->setBrush(QBrush(painter->pen().color()));
    painter->drawEllipse(-3, -3, 6, 6);
}

QRectF GraphicNode::boundingRect() const
{
    return {-3, -3, 6, 6};
}

}
