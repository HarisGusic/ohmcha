#ifndef GRAPHIC_RESISTOR_H
#define GRAPHIC_RESISTOR_H

#include "graphic_component.h"

namespace Ohmcha
{

class GraphicResistor : public GraphicComponent
{

public:

    // Constructors
    GraphicResistor();
    GraphicResistor(Resistor *resistor);
    GraphicResistor(QPointF node1, QPointF node2);
    GraphicResistor(const GraphicResistor &original);

    // Methods
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    virtual GraphicComponent *copy() const override;

    // Setters
    void setCenter(QPointF center) override;

    // Getters
    Resistor *getComponent();

private:

    // Attributes

    float terminalSize = 10;
    QPen pen;
};

}

#endif // GRAPHIC_RESISTOR_H
