#include "graphic_node.h"
#include "graphic_branch.h"

#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>
#include <QDebug>

namespace Ohmcha
{

GraphicNode::GraphicNode()
{
    setZValue(100);
    component = new Node;
    terminals = {{0, 0}};
}

GraphicNode::GraphicNode(Node *node)
    : GraphicNode()
{
    component = node;
    synchronize();
}

GraphicNode::GraphicNode(const GraphicNode &original)
    : GraphicComponent(original)
{

}

GraphicNode::~GraphicNode()
{
    //TODO make more elegant
}

void GraphicNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    applyColors(painter, option);

    painter->setBrush(QBrush(painter->pen().color()));
    painter->drawEllipse(-3, -3, 6, 6);
}

QRectF GraphicNode::boundingRect() const
{
    return {-3, -3, 6, 6};
}

GraphicComponent *GraphicNode::copy() const
{
    return new GraphicNode(*this);
}

void GraphicNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    GraphicComponent::mousePressEvent(event);
    qInfo() << QString::fromStdString(getComponent()->getName());
}

}
