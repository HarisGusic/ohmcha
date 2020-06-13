#ifndef CIRCUITVIEW_H
#define CIRCUITVIEW_H

#include "graphic_component.h"

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
    // Horizontal and vertical guides attached to the cursor
    QGraphicsLineItem *hGuide{}, *vGuide{};
    GraphicComponent *pendingInsert{};
    // Position where a drag has started, in original coordinates (provided by event)
    QPointF _dragPos;
    bool _dragging = false;

public:

    enum Mode {Idle, InsertDragDrop, InsertPoints};

protected:

    Mode mode = Idle;
    void *editBuffer = nullptr;
    float gridSpacingX = 20, gridSpacingY = 20;
    bool snapOn = false, showGrid = true;
    // The cursor position in scene coordinates, before applying snap to grid
    QPointF rawCursorPos;
    // The internally tracked cursor position in scene coordinates, affected by snap
    QPointF cursorPos;
    bool _selectionModeDetermined = false;

public:
    // Constructors
    CircuitView(QWidget *parent = nullptr);

    // Methods
    void insertComponent(GraphicComponent *component, Mode insertMode);
    void initiateInsertComponent(GraphicComponent *component, Mode insertMode);

    void zoomIn(float scale = 1.2);
    void resetZoom();

    /**
     * Show/hide the grid.
     */
    void setGridVisibility(bool visibility);
    /**
     * Enable/disable snap to grid.
     */
    void setSnap(bool state);

    QPointF getCursorPosition();

private:
    /**
     * Update cursor guides' position based on the current cursor position.
     */
    void updateCursorGuides();
    /**
     * Change the internally tracked cursor position to make it snap to grid.
     */
    void snapToGrid();

signals:
    /**
     * Called when a component is successfully inserted into a view.
     */
    void componentInserted();
    /**
     * Emitted when a component is selected individually.
     */
    void componentSelected(GraphicComponent *);

protected:

    // Event handling
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

};

}

#endif // CIRCUITVIEW_H
