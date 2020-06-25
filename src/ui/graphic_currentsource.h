#ifndef OHMCHA_GRAPHICCURRENTSOURCE_H
#define OHMCHA_GRAPHICCURRENTSOURCE_H

#include "graphic_component.h"

namespace Ohmcha {

class GraphicCurrentSource : public GraphicComponent
{

public:
    // Constructors
    GraphicCurrentSource();
    GraphicCurrentSource(CurrentSource *source);
    GraphicCurrentSource(const GraphicCurrentSource &original);

    // Methods
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    virtual GraphicComponent *copy() const override;

private:

    // Attributes
    QPen pen;

};

}

#endif
