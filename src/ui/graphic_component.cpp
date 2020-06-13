#include "graphic_component.h"

#include "graphic_resistor.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

namespace Ohmcha
{

GraphicComponent::GraphicComponent()
{
    setAcceptHoverEvents(true);
    setFlag(ItemIsSelectable);
    setFlag(ItemIsMovable);
}

GraphicComponent::GraphicComponent(const GraphicComponent &original)
    : GraphicComponent()
{
    terminals = original.terminals;
    size = original.size;
    component = original.component->copy();
    textAnchor = original.textAnchor;
    textPos = original.textPos;
    textAngle = original.textAngle;
    textRotationIndependent = original.textRotationIndependent;
    setTransform(original.transform());
    setRotation(original.rotation());
    setName(original.getName());
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

void GraphicComponent::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverEnterEvent(event);
    scene()->update();
}

void GraphicComponent::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverLeaveEvent(event);
    scene()->update();
}

}
