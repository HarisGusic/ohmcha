#include "graphic_component.h"

#include "graphic_resistor.h"
#include "graphic_branch.h"
#include "circuitview.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>

namespace Ohmcha
{

GraphicComponent::GraphicComponent()
{
    setAcceptHoverEvents(true);
    setZValue(25);
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

const QPointF *GraphicComponent::findNearestTerminal(QPointF point) const
{
    for (const QPointF &terminal : terminals)
        // TODO make it possible to set this value somewhere.
        if (QPointF::dotProduct(point - terminal, point - terminal) <= 25)
            return &terminal;
    return nullptr;
}

GraphicComponent *GraphicComponent::newFromComponent(Component *component)
{
    if (dynamic_cast<Resistor*>(component))
        return new GraphicResistor();
    //TODO add others
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

QString GraphicComponent::getName() const
{
    return QString::fromStdString(component->getName());
}

QPointF GraphicComponent::getCenter() const
{
    return pos() + QPointF(size.width()/2, size.height()/2);
}

GraphicComponent::Anchor GraphicComponent::getTextAnchor() const
{
    return textAnchor;
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

CircuitViewScene *GraphicComponent::getScene() const
{
    return dynamic_cast<CircuitViewScene*>(scene());
}

void GraphicComponent::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverEnterEvent(event);
    scene()->update();
}

void GraphicComponent::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if ((getScene() && getScene()->isInsertingComponent()) ||
            (!scene()->selectedItems().empty() && *scene()->selectedItems().begin() == this))
    {
        auto nearestTerminal = findNearestTerminal(event->pos());
        if (nearestTerminal != _selectedTerminal)
        {
            _selectedTerminal = nearestTerminal;
            scene()->update();
        }
    }
}

void GraphicComponent::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverLeaveEvent(event);
    _selectedTerminal = nullptr;

    scene()->update();
}

void GraphicComponent::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (_selectedTerminal && getScene())
    { // A terminal on this item was clicked
        event->ignore();
        getScene()->terminalClickEvent(this, *_selectedTerminal);
    }
}

}
