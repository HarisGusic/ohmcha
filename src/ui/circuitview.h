#ifndef CIRCUITVIEW_H
#define CIRCUITVIEW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsItem>

namespace Ohmcha
{

class Component;

class CircuitView : public QGraphicsView
{
    Q_OBJECT

private:

    float zoomLevel = 1;
    // Horizontal and vertical guides attached to the cursor.
    QGraphicsLineItem *hGuide{}, *vGuide{};
    QPointF _dragPos;
    bool _dragging;

public:

    enum Mode {Idle, InsertDragDrop, InsertPoints};

protected:

    Mode mode = Idle;
    void *editBuffer = nullptr;
    float gridSpacingX = 20, gridSpacingY = 20;
    bool snapOn = false, showGrid = true;
    // The cursor position in scene coordinates, before applying snap to grid
    QPointF rawCursorPos;
    // The cursor position in scene coordinates, affected by snap
    QPointF cursorPos;

public:
    // Constructors
    CircuitView(QWidget *parent = nullptr);

    // Methods
    void drawLine(float x1, float y1, float x2, float y2);
    void insertComponent(Component *component, Mode insertMode);

    void zoomIn(float scale = 1.2);
    void resetZoom();

    void setGridVisibility(bool visibility);
    /**
     * Enable/disable snap to grid.
     */
    void setSnap(bool state);

    QPointF getCursorPosition();

private:
    void updateCursorGuides();
    void snapToGrid();

protected:

    void  (*mouseCallback)(QMouseEvent *event, CircuitView *owner);

    // Event handling
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

};

}

#endif // CIRCUITVIEW_H
