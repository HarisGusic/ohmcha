#ifndef COMPONENTPREVIEW_H
#define COMPONENTPREVIEW_H

#include <QRadioButton>
#include <QWidget>

#include "circuitview.h"
#include "graphic_component.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ComponentPreview; }
QT_END_NAMESPACE

namespace Ohmcha
{

class CircuitView;
class GraphicComponent;

class ComponentPreview : public QWidget
{
    Q_OBJECT

public:

    // Constructors
    explicit ComponentPreview(QWidget *parent = nullptr);
    ~ComponentPreview();

    // Methods
    /**
     * Connect this preview to the active circuit view.
     * Call this when first showing this widget or when
     * the active circuit view changes.
     */
    void initialize();
    /**
     * Create a new GraphicComponent, with the same properties as the previous component.
     */
    void initializeNewComponent();
    /**
     * Synchronize the contents of the text boxes and buttons
     * with the actual properties of component.
     */
    void synchronize();

    // Setters
    void setCircuitView(CircuitView *cv);

public slots:
    void setEditExisting(GraphicComponent *component);

private slots:

    /** Called when the selection in the circuit view changes */
    void on_selectionChanged();

    void on_textAnchorPicked(int id);
    void on_textIndependencePicked(int id);

    void on_btnAdd_clicked();
    void on_editAngle_textEdited(const QString &s);
    void on_editText_textEdited(const QString &s);
    void on_editTextAngle_textEdited(const QString &s);

private:

    // Private methods
    /**
     * Call this when component is modified.
     */
    void updatePreview();

    // Attributes

    Ui::ComponentPreview *ui;
    /** component is guaranteed to be non-null whenever this widget is visible. */
    GraphicComponent *component = nullptr;
    /** The circuit view that is currently active in the window */
    CircuitView *circuitView = nullptr;
    /** A 3x3 grid of buttons used to choose the text anchor */
    QRadioButton anchors[3][3];
    QButtonGroup *anchorGroup{};
};

}
#endif // COMPONENTPREVIEW_H
