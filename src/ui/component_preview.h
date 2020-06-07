#ifndef COMPONENTPREVIEW_H
#define COMPONENTPREVIEW_H

#include <QRadioButton>
#include <QWidget>

#include "src/model/component.h"
#include "circuitview.h"

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
    void setComponent(Component *component);
    void setVisible(bool visible) override;
    void setCircuitView(CircuitView *cv);

private slots:
    void on_btnAdd_clicked();
    void componentInserted();

private:
    Ui::ComponentPreview *ui;
    Component *component = nullptr;
    CircuitView *circuitView = nullptr;
    // Are we creating a new component or editing an existing one?
    bool newComponent = true;
    QRadioButton anchors[3][3];
};

}
#endif // COMPONENTPREVIEW_H
