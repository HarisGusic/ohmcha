#ifndef GRAPHICBRANCH_H
#define GRAPHICBRANCH_H

#include "graphic_component.h"

namespace Ohmcha
{

class GraphicBranch : public GraphicComponent
{

public:

    // Constructors
    GraphicBranch();

    // Methods
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;

    // Setters
    void setFirstAnchor(GraphicComponent *item, const QPointF &point);
    void setSecondAnchor(GraphicComponent *item, const QPointF &point);

    // Getters
    GraphicComponent *getFirstAnchor() const;
    QPointF getFirstAnchorPoint() const;
    QPointF getSecondAnchorPoint() const;
    GraphicComponent *getSecondAnchor() const;


private:

    // Attributes

    /** The components to which this branch is attached */
    GraphicComponent *first{}, *second{};
    /** The points on first/second to which this branch is attached, in their respective coordinate systems. */
    QPointF pFirst, pSecond;
    QPen pen;

};

}

#endif // GRAPHICBRANCH_H
