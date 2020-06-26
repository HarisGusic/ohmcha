#ifndef CIRCUITVIEW_H
#define CIRCUITVIEW_H

#include "graphic_component.h"
#include "component_preview.h"
#include "graphic_branch.h"
#include "src/model/schematic.h"

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsItem>

namespace Ohmcha
{

class Component;
class GraphicBranch;
class ComponentPreview;
class CircuitView;

class CircuitViewScene : public QGraphicsScene
{
    Q_OBJECT
public:
    // Constructors
    CircuitViewScene(CircuitView *circuitView);

    // Methods
    void clear();

    // Getters
    bool isInsertingComponent() const;

    // Event handling
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

    void terminalClickEvent(GraphicComponent *source, QPointF terminal);

private slots:

    void on_selectionChanged();

private:
    GraphicBranch *_insertedBranch{};
    CircuitView *circuitView;
};

class CircuitView : public QGraphicsView
{
    Q_OBJECT

public:

    enum Mode {Idle, InsertDragDrop, InsertPoints};

    // Constructors
    CircuitView(QWidget *parent = nullptr);
    CircuitView(QWidget *parent, Schematic *schematic);

    ~CircuitView();

    // Methods
    void initialize();
    void initiateInsertComponent(GraphicComponent *component, Mode insertMode);

    void zoomIn(float scale = 1.2);
    void resetZoom();

    CircuitViewScene *scene();

    // Setters
    /**
     * Show/hide the grid.
     */
    void setGridVisibility(bool visibility);
    /**
     * Enable/disable snap to grid.
     */
    void setSnap(bool state);
    void setComponentPreview(ComponentPreview *componentPreview);

    // Getters
    /**
     * Return the cursor position with any snap-to-grid applied.
     */
    QPointF getCursorPosition();
    ComponentPreview *getComponentPreview();
    /**
     * Return the Schematic representation of this circuit. The result
     * is guaranteed to be non-null.
     */
    Schematic *getSchematic();


signals:
    /**
     * Emitted when a component is selected individually.
     */
    void componentSelected(GraphicComponent *);
    void itemDeleted(GraphicComponent *);

private:

    // Event handling
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    void resizeEvent(QResizeEvent *event) override;

    // Private methods
    /**
     * Update cursor guides' position based on the current cursor position.
     */
    void updateCursorGuides();
    /**
     * Change the internally tracked cursor position to make it snap to grid.
     */
    void snapToGrid();

    // Attributes

    float zoomLevel = 1;
    // Horizontal and vertical guides attached to the cursor
    QGraphicsLineItem *hGuide{}, *vGuide{};
    GraphicComponent *pendingInsert{};
    ComponentPreview *componentPreview{};
    Schematic *schematic{};

    Mode mode = Idle;

    // The cursor position in scene coordinates, before applying snap to grid
    QPointF rawCursorPos;
    // The internally tracked cursor position in scene coordinates, affected by snap
    QPointF cursorPos;
    float gridSpacingX = 20, gridSpacingY = 20;
    bool snapOn = false, showGrid = true;

    bool _selectionModeDetermined = false;
    // Position where a drag has started, in original coordinates (provided by event)
    QPointF _dragPos;
    bool _dragging = false;

    friend class CircuitViewScene;

};

}

#endif // CIRCUITVIEW_H
