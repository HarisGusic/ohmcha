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
    void setComponent(GraphicComponent *component);
    void setVisible(bool visible) override;
    void setCircuitView(CircuitView *cv);

private slots:
    void on_btnAdd_clicked();
    void textAnchorPicked(int id);
    void componentInserted();

    void on_editText_textEdited(const QString &arg1);

private:
    Ui::ComponentPreview *ui;
    GraphicComponent *component = nullptr;
    CircuitView *circuitView = nullptr;
    // Are we creating a new component or editing an existing one?
    bool newComponent = true;
    QRadioButton anchors[3][3];
};

}
#endif // COMPONENTPREVIEW_H
