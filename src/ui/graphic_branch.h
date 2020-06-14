#ifndef GRAPHICBRANCH_H
#define GRAPHICBRANCH_H

#include "graphic_component.h"

namespace Ohmcha
{

class GraphicComponent;

class GraphicBranch : public GraphicComponent
{

public:

    // Constructors
    GraphicBranch();
    GraphicBranch(Branch *branch);

    // Methods
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void split(const QPointF &point);

    // Setters
    /**
     * Point is specified in item's coordinate system. If item is null, then
     * the point is specified in scene coordinates. This is useful in the
     * interim while connecting two components together.
     */
    void setFirstAnchor(GraphicComponent *item, const QPointF &point);
    /** See setFirstAnchor */
    void setSecondAnchor(GraphicComponent *item, const QPointF &point);

    // Getters
    GraphicComponent *getFirstAnchor() const;
    QPointF getFirstAnchorPoint() const;
    QPointF getSecondAnchorPoint() const;
    GraphicComponent *getSecondAnchor() const;

    // Event handling
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

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
