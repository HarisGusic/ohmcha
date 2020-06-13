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

class ComponentPreview : public QWidget
{
    Q_OBJECT

public:
    explicit ComponentPreview(QWidget *parent = nullptr);
    ~ComponentPreview();
    void setCircuitView(CircuitView *cv);
    void initialize();
    /**
     * Synchronize the contents of the text boxes and buttons
     * with the actual properties of component.
     */
    void synchronize();

public slots:
    void setEditExisting(GraphicComponent *component);

private slots:
    void on_btnAdd_clicked();
    void textAnchorPicked(int id);
    void textIndependencePicked(int id);
    void componentInserted();

    void on_editAngle_textEdited(const QString &s);
    void on_editText_textEdited(const QString &s);
    void on_editTextAngle_textEdited(const QString &s);

private:

    void initializeNewComponent();
    void updatePreview();

    Ui::ComponentPreview *ui;
    // Component is guaranteed to be non-null whenever this widget is visible
    GraphicComponent *component = nullptr;
    CircuitView *circuitView = nullptr;
    // Are we creating a new component or editing an existing one?
    bool newComponent = true;
    QRadioButton anchors[3][3];
};

}
#endif // COMPONENTPREVIEW_H
