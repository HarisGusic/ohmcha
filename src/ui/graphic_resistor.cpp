#include "graphic_resistor.h"

#include <QStyleOptionGraphicsItem>
#include <QStyle>
#include <QPainter>
#include <QGraphicsScene>

namespace Ohmcha
{

GraphicResistor::GraphicResistor()
    : GraphicComponent()
{
    component = new Resistor;
    terminals = { {0, -size.height() / 2}, {0, size.height() / 2} };
    textPos = {15, 0};
}

GraphicResistor::GraphicResistor(Resistor *resistor)
    : GraphicResistor()
{
    component = resistor;
}

GraphicResistor::GraphicResistor(QPointF node1, QPointF node2)
    : GraphicResistor()
{
    setPos((node1 + node2) / 2);
}

GraphicResistor::GraphicResistor(const GraphicResistor &original)
    : GraphicComponent(original)
{

}

QRectF GraphicResistor::boundingRect() const
{
    return {
        -size.width()/2 - pen.width()/2,
        -size.height()/2 - pen.width()/2 - 4, // 4 is the radius of a terminal
        size.width() + pen.width(),
        size.height() + pen.width() + 8
    };
}

void GraphicResistor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!component) return;

    applyColors(painter, option);

    // Draw circle when cursor is near terminal
    if (_selectedTerminal)
    {
        painter->save();
        painter->setBrush(QBrush(painter->pen().color()));
        painter->drawEllipse(*_selectedTerminal, 4, 4);
        painter->restore();
    }

    // Draw terminals
    painter->drawLine(0, -size.height()/2, 0, -size.height()/2 + terminalSize);
    painter->drawLine(0, size.height()/2 - terminalSize, 0, size.height()/2);
    // Draw main rect
    QRectF rect = {-size.width()/2, -size.height()/2 + terminalSize, size.width(), size.height() - 2*terminalSize};
    painter->drawRect(rect);

    drawText(painter);

}

GraphicComponent *GraphicResistor::copy() const
{
    return new GraphicResistor(*this);
}

void GraphicResistor::setCenter(QPointF center)
{
    GraphicComponent::setCenter(center);
}

Resistor *GraphicResistor::getComponent()
{
    return dynamic_cast<Resistor*>(component);
}

}
