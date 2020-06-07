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

protected:
    // List of terminals in scene coordinates
    QList<QPointF> terminals;
    Component *component;

public:
    GraphicComponent();

    virtual float getTerminalSpan() = 0;

    static GraphicComponent *newFromComponent(Component *component);
};

class GraphicResistor : public GraphicComponent
{
private:

    QSizeF size = {14, 50};
    float terminalSize = 8;
    QPen pen;

public:

    GraphicResistor(QPointF node1, QPointF node2);
    GraphicResistor();

    // Getters
    Resistor *getComponent();
    float getTerminalSpan();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

}
#endif // GRAPHICCOMPONENT_H
