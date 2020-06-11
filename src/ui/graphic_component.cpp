#include "graphic_component.h"

#include <QPainter>

namespace Ohmcha
{

GraphicComponent::GraphicComponent()
{

}

GraphicComponent::Anchor GraphicComponent::getTextAnchor() const
{
    return textAnchor;
}

QPointF GraphicComponent::getCenter() const
{
    return pos() + QPointF(size.width()/2, size.height()/2);
}

void GraphicComponent::setTextAnchor(GraphicComponent::Anchor anchor)
{
    textAnchor = anchor;
}

void GraphicComponent::setCenter(QPointF center)
{
    this->setPos(center - QPointF(size.width(), size.height()));
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
    // Draw terminals
    painter->drawLine(0, -size.height()/2, 0, -size.height()/2 + terminalSize);
    painter->drawLine(0, size.height()/2 - terminalSize, 0, size.height()/2);

    // Draw main rect
    QRectF rect = {-size.width()/2, -size.height()/2 + terminalSize, size.width(), size.height() - 2*terminalSize};
    painter->drawRect(rect);

    // Draw text
    QFontMetricsF fm(painter->fontMetrics());
    float width=fm.width("R"), height = fm.height();
    // TODO right now I have to divide height by 2 for the text to display correctly.
    // Find a fix. It is probably because I have to make a coordinate transformation somewhere.
    QPointF _textPos = textPos - QPointF((textAnchor % 3) * width / 2, (textAnchor / 3 - 2) * height / 2 / 2);
    painter->drawText(_textPos, "R");
}

}
