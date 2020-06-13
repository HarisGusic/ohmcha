#ifndef GRAPHICCOMPONENT_H
#define GRAPHICCOMPONENT_H

#include <QObject>
#include <QGraphicsItem>
#include <QPen>

#include "src/model/component.h"

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
    float textAngle = 0;
    bool textRotationIndependent = false;
    const QPointF *_selectedTerminal = nullptr;

public:
    GraphicComponent();
    GraphicComponent(const GraphicComponent &original);

    QString getName() const;
    QPointF getCenter() const;
    Anchor getTextAnchor() const;
    QPointF getTextPosition() const;
    float getTextRotation() const;
    bool isTextRotationIndependent() const;
    /**
     * Return the terminal that is within a certain distance
     * to the specified point. If there is no such terminal, return null.
     * The point is specified in this item's coordinate system.
     */
    const QPointF *findNearestTerminal(QPointF point) const;

    void setName(QString name);
    virtual void setCenter(QPointF center);
    void setTextAnchor(Anchor anchor);
    void setTextPosition(QPointF pos);
    void setTextRotation(float angle);
    void setTextRotationIndependent(bool independent);

    void onTerminalClicked(QPointF position);

    /**
     * Dynamically allocate a GraphicComponent wrapper around
     * the specified Component. The type of the resulting object
     * is determined based on the type of Component.
     * TODO: implement.
     */
    static GraphicComponent *newFromComponent(Component *component);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};

}
#endif // GRAPHICCOMPONENT_H
