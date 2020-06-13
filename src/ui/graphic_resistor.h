#ifndef GRAPHIC_RESISTOR_H
#define GRAPHIC_RESISTOR_H

#include "graphic_component.h"

namespace Ohmcha
{

class GraphicResistor : public GraphicComponent
{
    QSizeF size = {20, 70};
    float terminalSize = 10;
    QPen pen;

public:

    GraphicResistor(QPointF node1, QPointF node2);
    GraphicResistor();
    GraphicResistor(const GraphicResistor &original);

    // Getters
    Resistor *getComponent();

    // Setters
    void setCenter(QPointF center) override;

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};

}

#endif // GRAPHIC_RESISTOR_H
