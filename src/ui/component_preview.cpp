#include "component_preview.h"
#include "ui_component_preview.h"
#include "src/ui/mainwindow.h"

#include <QGridLayout>

namespace Ohmcha
{

ComponentPreview::ComponentPreview(QWidget *parent)
    : QWidget(parent), ui(new Ui::ComponentPreview)
{
    ui->setupUi(this);
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
        {
            anchors[i][j].setText("");
            ui->anchorPicker->addWidget(&anchors[i][j], i, j);
        }
    anchors[1][0].setChecked(true);
}

ComponentPreview::~ComponentPreview()
{
    delete ui;
}

void ComponentPreview::setComponent(Component *component)
{
    this->component = component;
    newComponent = false;
    ui->btnAdd->setVisible(false);
}

void ComponentPreview::setVisible(bool visible)
{
    QWidget::setVisible(visible);
    if (component == nullptr)
        component = Component::newByName("Resistor"); //TODO generalize
}

void ComponentPreview::setCircuitView(CircuitView *cv)
{
    circuitView = cv;
}

void ComponentPreview::on_btnAdd_clicked()
{
    circuitView->insertComponent(component, CircuitView::InsertPoints);
    connect(circuitView, &CircuitView::componentInserted, this, &ComponentPreview::componentInserted);
}

void ComponentPreview::componentInserted()
{
    component = Component::newByName("Resistor"); //TODO: generalize
    disconnect(circuitView, &CircuitView::componentInserted, this, &ComponentPreview::componentInserted);
}

}

