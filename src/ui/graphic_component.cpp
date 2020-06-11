#include "graphic_component.h"

#include <QPainter>

namespace Ohmcha
{

GraphicComponent::GraphicComponent()
{

}

QString GraphicComponent::getName() const
{
    return QString::fromStdString(component->getName());
}

GraphicComponent::Anchor GraphicComponent::getTextAnchor() const
{
    return textAnchor;
}

QPointF GraphicComponent::getCenter() const
{
    return pos() + QPointF(size.width()/2, size.height()/2);
}

QPointF GraphicComponent::getTextPosition() const
{
    return textPos;
}

float GraphicComponent::getTextRotation() const
{
    return textAngle;
}

bool GraphicComponent::isTextRotationIndependent() const
{
    return textRotationIndependent;
}

void GraphicComponent::setTextAnchor(GraphicComponent::Anchor anchor)
{
    textAnchor = anchor;
}

void GraphicComponent::setTextPosition(QPointF pos)
{
    textPos = pos;
}

void GraphicComponent::setTextRotation(float angle)
{
    textAngle = angle;
}

void GraphicComponent::setTextRotationIndependent(bool independent)
{
    textRotationIndependent = independent;
}

void GraphicComponent::setCenter(QPointF center)
{
    this->setPos(center - QPointF(size.width(), size.height()));
}

void GraphicComponent::setName(QString name)
{
    component->setName(name.toStdString());
}

GraphicComponent *GraphicComponent::newFromComponent(Component *component)
{
    if (dynamic_cast<Resistor*>(component))
        return new GraphicResistor();
    //TODO add others
}

GraphicResistor::GraphicResistor(QPointF node1, QPointF node2)
    : GraphicResistor()
{
    setPos((node1 + node2) / 2);
}

GraphicResistor::GraphicResistor()
    : GraphicComponent()
{
    component = new Resistor;
}

Resistor *GraphicResistor::getComponent()
{
    return dynamic_cast<Resistor*>(component);
}

float GraphicResistor::getTerminalSpan() const
{
    return size.width();
}

QRectF GraphicResistor::boundingRect() const
{
    return {-size.width()/2 - pen.width()/2, -size.height()/2 - pen.width()/2, size.width() - pen.width(), size.height() - pen.width()};
}

void GraphicResistor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!component) return;
    // Draw terminals
    painter->drawLine(0, -size.height()/2, 0, -size.height()/2 + terminalSize);
    painter->drawLine(0, size.height()/2 - terminalSize, 0, size.height()/2);

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
