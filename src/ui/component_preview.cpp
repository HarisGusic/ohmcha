#include "component_preview.h"
#include "graphic_component.h"
#include "ui_component_preview.h"
#include "src/ui/mainwindow.h"

#include <QButtonGroup>
#include <QGridLayout>

namespace Ohmcha
{

ComponentPreview::ComponentPreview(QWidget *parent)
    : QWidget(parent), ui(new Ui::ComponentPreview)
{
    ui->setupUi(this);
    ui->preview->setScene(new QGraphicsScene);
    QButtonGroup *btnGroup = new QButtonGroup;
    btnGroup->setExclusive(true);
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
        {
            anchors[i][j].setText("");
            ui->anchorPicker->addWidget(&anchors[i][j], i, j);
            btnGroup->addButton(&anchors[i][j], 3*i + j);
        }
    connect(btnGroup, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this, &ComponentPreview::textAnchorPicked);
    anchors[1][0].setChecked(true);
}

ComponentPreview::~ComponentPreview()
{
    delete ui;
}

void ComponentPreview::setComponent(GraphicComponent *component)
{
    this->component = component;
    newComponent = false;
    ui->btnAdd->setVisible(false);
}

void ComponentPreview::setVisible(bool visible)
{
    QWidget::setVisible(visible);
    if (!visible) return;
    if (component == nullptr)
        component = new GraphicResistor({},{}); //TODO generalize
    ui->preview->scene()->addItem(component); //TODO generalize
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

void ComponentPreview::textAnchorPicked(int id)
{
    component->setTextAnchor(GraphicComponent::Anchor(id));
    // TODO find a better way to update the component
    ui->preview->scene()->removeItem(component);
    ui->preview->scene()->addItem(component);
}

void ComponentPreview::componentInserted()
{
    component = new GraphicResistor(); //TODO: generalize
    ui->preview->scene()->addItem(component);
    disconnect(circuitView, &CircuitView::componentInserted, this, &ComponentPreview::componentInserted);
}

void ComponentPreview::on_editText_textEdited(const QString &arg1)
{
    component->setName(ui->editText->text());
    // TODO find a better way to update the component
    ui->preview->scene()->removeItem(component);
    ui->preview->scene()->addItem(component);
}

}
