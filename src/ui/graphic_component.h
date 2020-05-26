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
};

class GraphicResistor : public GraphicComponent
{
private:

    QSizeF size = {8, 30};
    float terminalSize = 5;
    QPen pen;

public:

    GraphicResistor(QPointF node1, QPointF node2);

    // Getters
    Resistor *getComponent();
    float getTerminalSpan();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

}
#endif // GRAPHICCOMPONENT_H
