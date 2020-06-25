#include "graphic_component.h"

#include "graphic_resistor.h"
#include "graphic_branch.h"
#include "graphic_emf.h"
#include "graphic_currentsource.h"
#include "circuitview.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

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

void GraphicComponent::synchronize()
{
    auto *pos = component->getPosition();
    if (pos != nullptr)
        setCenter({pos->x, pos->y});
    setRotation(component->getAngle());
    setName(QString::fromStdString(component->getName()));
    auto tpos = component->getTextPos();
    setTextPosition({tpos.x, tpos.y});
    setTextRotation(component->getTextAngle());
    setTextRotationIndependent(component->isTextOrientationIndependent());
    setTextAnchor((Anchor) component->getTextAnchor());
}

GraphicComponent *GraphicComponent::newFromComponent(Component *component)
{
    if (dynamic_cast<Resistor*>(component))
        return new GraphicResistor((Resistor*) component);
    else if (dynamic_cast<Emf*>(component))
        return new GraphicEmf((Emf*) component);
    else if (dynamic_cast<CurrentSource*>(component))
        return new GraphicCurrentSource((CurrentSource*) component);
    //TODO add others
}

void GraphicComponent::setTextAnchor(GraphicComponent::Anchor anchor)
{
    textAnchor = anchor;
    //TODO Change the way this is done
    component->setTextAnchor(anchor);
}

void GraphicComponent::setTextPosition(QPointF pos)
{
    textPos = pos;
    component->setTextPos({(float) pos.x(), (float) pos.y()});
}

void GraphicComponent::setTextRotation(float angle)
{
    textAngle = angle;
    component->setTextAngle(angle);
}

void GraphicComponent::setTextRotationIndependent(bool independent)
{
    textRotationIndependent = independent;
    component->setTextOrientationIndependent(independent);
}

void GraphicComponent::setComponent(Component *component)
{
    this->component = component;
}

QSizeF GraphicComponent::getSize() const
{
    return size;
}

void GraphicComponent::setCenter(QPointF center)
{
    this->setPos(center);
    component->setPosition(new Component::Pos{(float) center.x(), (float) center.y()});
}

void GraphicComponent::setRotation(float angle)
{
    QGraphicsItem::setRotation(angle);
    component->setAngle(angle);
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

Component *GraphicComponent::getComponent() const
{
    return component;
}

CircuitViewScene *GraphicComponent::getScene() const
{
    return dynamic_cast<CircuitViewScene*>(scene());
}

int GraphicComponent::getTerminalId(const QPointF &terminal) const
{
    int i = 0;
    for(auto it = terminals.begin(); it != terminals.end(); ++it, ++i)
    {
        if (*it == terminal)
            return i;
    }
    return -1;
}

QList<QPointF> GraphicComponent::getTerminals() const
{
    return terminals;
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
    //TODO very inelegant -- change
    if (_selectedTerminal)
    {
        bool terminalAvailable = getTerminalId(*_selectedTerminal) == 0 ? !branch1 : !branch2;
        if (terminalAvailable && getScene())
        { // A terminal on this item was clicked
            event->ignore();
            getScene()->terminalClickEvent(this, *_selectedTerminal);
        }
    }
}

void GraphicComponent::applyColors(QPainter *painter, const QStyleOptionGraphicsItem *option)
{
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
}

void GraphicComponent::drawText(QPainter *painter)
{
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
