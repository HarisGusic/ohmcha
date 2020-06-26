#ifndef GRAPHICNODE_H
#define GRAPHICNODE_H

#include "graphic_component.h"

namespace Ohmcha
{

class GraphicNode : public GraphicComponent
{
public:
    // Constructors
    GraphicNode();
    GraphicNode(Node *node);
    GraphicNode(const GraphicNode &original);

    ~GraphicNode();

    // Methods
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;

    virtual GraphicComponent *copy() const override;

};

}

#endif // GRAPHICNODE_H
