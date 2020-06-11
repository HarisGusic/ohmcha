#ifndef GRAPHICCOMPONENT_H
#define GRAPHICCOMPONENT_H

#include <QObject>
#include <QGraphicsItem>
#include <QPen>

#include <src/model/component.h>

namespace Ohmcha
{

class GraphicComponent : public QGraphicsItem
{
public:
    enum Anchor { NW = 0, N, NE, W, C, E, SW, S, SE};
protected:
    // List of terminals in scene coordinates
    QList<QPointF> terminals;
    QSizeF size;
    Component *component = nullptr;
    Anchor textAnchor = W;
    QPointF textPos;

public:
    GraphicComponent();

    /**
     * Return the maximum distance between terminals
     */
    virtual float getTerminalSpan() = 0;
    QPointF getCenter() const;
    Anchor getTextAnchor() const;
    QPointF getTextPosition() const;

    void setName(QString name);
    void setCenter(QPointF center);
    void setTextAnchor(Anchor anchor);
    void setTextPosition(QPointF pos);

    /**
     * Dynamically allocate a GraphicComponent wrapper around
     * the specified Component. The type of the resulting object
     * is determined based on the type of Component.
     * TODO: implement.
     */
    static GraphicComponent *newFromComponent(Component *component);
};

class GraphicResistor : public GraphicComponent
{
    QSizeF size = {14, 50};
    float terminalSize = 8;
    QPen pen;

public:

    GraphicResistor(QPointF node1, QPointF node2);
    GraphicResistor();

    // Getters
    Resistor *getComponent();
    float getTerminalSpan() override;

    // Setters

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

}
#endif // GRAPHICCOMPONENT_H
