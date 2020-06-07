#include "graphic_component.h"

#include <QPainter>

namespace Ohmcha
{

GraphicComponent::GraphicComponent()
{

}

GraphicComponent *GraphicComponent::newFromComponent(Component *component)
{
    if (dynamic_cast<Resistor*>(component))
        return new GraphicResistor();
    //TODO add others
}

GraphicResistor::GraphicResistor(QPointF node1, QPointF node2)
{
    setPos((node1 + node2) / 2);
}

GraphicResistor::GraphicResistor()
    : GraphicComponent()
{

}

Resistor *GraphicResistor::getComponent()
{
    return dynamic_cast<Resistor*>(component);
}

float GraphicResistor::getTerminalSpan()
{
    return size.width();
}

QRectF GraphicResistor::boundingRect() const
{
    return {-size.width()/2 - pen.width()/2, -size.height()/2 - pen.width()/2, size.width() - pen.width(), size.height() - pen.width()};
}

void GraphicResistor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawLine(0, -size.height()/2, 0, -size.height()/2 + terminalSize);
    painter->drawLine(0, size.height()/2 - terminalSize, 0, size.height()/2);
    QRectF rect = {-size.width()/2, -size.height()/2 + terminalSize, size.width(), size.height() - 2*terminalSize};
    painter->drawRect(rect);
}

}
