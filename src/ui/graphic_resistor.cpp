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
}

GraphicResistor::GraphicResistor(const GraphicResistor &original)
    : GraphicComponent(original)
{

}

GraphicResistor::GraphicResistor(QPointF node1, QPointF node2)
    : GraphicResistor()
{
    setPos((node1 + node2) / 2);
}

Resistor *GraphicResistor::getComponent()
{
    return dynamic_cast<Resistor*>(component);
}

float GraphicResistor::getTerminalSpan() const
{
    return size.width();
}

void GraphicResistor::setCenter(QPointF center)
{
    GraphicComponent::setCenter(center);
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

    // Change color if item is hovered over
    if (option->state & QStyle::State_MouseOver)
    {
        auto pen = painter->pen();
        pen.setColor(QColor(0x8c,0x9e,0xff));
        painter->setPen(pen);
    }

    // Change color if item is selected
    if (option->state & QStyle::State_Selected)
    {
        auto pen = painter->pen();
        pen.setColor(QColor(0xff,0x8c,0x8c));
        painter->setPen(pen);
    }

    // Draw terminals
    painter->drawLine(0, -size.height()/2, 0, -size.height()/2 + terminalSize);
    painter->drawLine(0, size.height()/2 - terminalSize, 0, size.height()/2);

    // Draw circle when cursor is near terminal
    if (_selectedTerminal)
    {
        painter->save();
        painter->setBrush(QBrush(painter->pen().color()));
        painter->drawEllipse(*_selectedTerminal, 4, 4);
        painter->restore();
    }

    // Draw main rect
    QRectF rect = {-size.width()/2, -size.height()/2 + terminalSize, size.width(), size.height() - 2*terminalSize};
    painter->drawRect(rect);

    // Counter-rotate text to make it independent from the component's rotation.
    if (textRotationIndependent)
        painter->rotate(-rotation());

    painter->rotate(textAngle);

    // Draw text
    QFontMetricsF fm(painter->fontMetrics());
    float width=fm.width(QString::fromStdString(component->getName())), height = fm.height();
    // TODO right now I have to divide height by 2 for the text to display correctly.
    // Find a fix. It is probably because I have to make a coordinate transformation somewhere.
    QPointF _textPos = textPos - QPointF((textAnchor % 3) * width / 2, (textAnchor / 3 - 2) * height / 2 / 2);
    painter->drawText(_textPos, QString::fromStdString(component->getName()));

}

}
