#ifndef OHMCHA_GRAPHICEMF_H
#define OHMCHA_GRAPHICEMF_H

#include "graphic_component.h"

namespace Ohmcha {

class GraphicEmf : public GraphicComponent
{
public:

    // Constructors
    GraphicEmf();
    GraphicEmf(Emf *emf);
    GraphicEmf(const GraphicEmf &original);

    // Methods
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    virtual GraphicComponent *copy() const override;

private:
    QPen pen;

};

}

#endif
