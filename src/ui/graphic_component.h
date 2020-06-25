#ifndef GRAPHICCOMPONENT_H
#define GRAPHICCOMPONENT_H

#include <QObject>
#include <QGraphicsItem>
#include <QPen>

#include "src/model/component.h"

namespace Ohmcha
{

class CircuitViewScene;

class GraphicComponent : public QGraphicsItem
{
public:

    enum Anchor { NW = 0, N, NE, W, C, E, SW, S, SE};

    // Constructors
    GraphicComponent();
    GraphicComponent(const GraphicComponent &original);

    // Methods
    /**
     * Return the terminal that is within a certain distance
     * to the specified point. If there is no such terminal, return null.
     * The point is specified in this item's coordinate system.
     */
    const QPointF *findNearestTerminal(QPointF point) const;
    virtual GraphicComponent *copy() const = 0;
    virtual void synchronize();

    /**
     * Dynamically allocate a GraphicComponent wrapper around
     * the specified Component. The type of the resulting object
     * is determined based on the type of Component.
     * TODO: implement.
     */
    static GraphicComponent *newFromComponent(Component *component);

    // Setters
    void setName(QString name);
    virtual void setCenter(QPointF center);
    void setRotation(float angle);
    void setTextAnchor(Anchor anchor);
    void setTextPosition(QPointF pos);
    void setTextRotation(float angle);
    void setTextRotationIndependent(bool independent);
    void setComponent(Component *component);

    // Getters
    virtual QSizeF getSize() const;
    QString getName() const;
    QPointF getCenter() const;
    Anchor getTextAnchor() const;
    QPointF getTextPosition() const;
    float getTextRotation() const;
    bool isTextRotationIndependent() const;
    Component *getComponent() const;
    CircuitViewScene *getScene() const;

    // Event handling
    void onTerminalClicked(QPointF position);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

protected:

    // Methods
    virtual void applyColors(QPainter *painter, const QStyleOptionGraphicsItem *option);
    virtual void drawText(QPainter *painter);

    // Attributes

    // List of terminals in scene coordinates
    QList<QPointF> terminals;
    QSizeF size;
    Component *component = nullptr;
    Anchor textAnchor = W;
    QPointF textPos;
    float textAngle = 0;
    bool textRotationIndependent = false;
    const QPointF *_selectedTerminal = nullptr;
};

}
#endif // GRAPHICCOMPONENT_H
