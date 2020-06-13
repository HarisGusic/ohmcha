#include "component_preview.h"

#include "graphic_resistor.h"
#include "ui_component_preview.h"
#include "mainwindow.h"

#include <QButtonGroup>
#include <QGridLayout>
#include <QScrollBar>

namespace Ohmcha
{

ComponentPreview::ComponentPreview(QWidget *parent)
    : QWidget(parent), ui(new Ui::ComponentPreview)
{
    ui->setupUi(this);

    // Setup preview widget
    ui->preview->setScene(new QGraphicsScene);
    ui->preview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->preview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->preview->verticalScrollBar()->blockSignals(true);
    ui->preview->horizontalScrollBar()->blockSignals(true);

    // Setup anchor picker and enable callbacks
    QButtonGroup *anchorGroup = new QButtonGroup;
    anchorGroup->setExclusive(true);
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
        {
            anchors[i][j].setText("");
            ui->anchorPicker->addWidget(&anchors[i][j], i, j);
            anchorGroup->addButton(&anchors[i][j], 3*i + j);
        }
    connect(anchorGroup, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this, &ComponentPreview::on_textAnchorPicked);
    anchors[1][0].setChecked(true);

    // Set callback for text independence picker
    QButtonGroup *textIndependenceGroup = new QButtonGroup;
    textIndependenceGroup->addButton(ui->btnDepend, 0);
    textIndependenceGroup->addButton(ui->btnIndependent, 1);
    ui->btnDepend->setChecked(true);
    connect(textIndependenceGroup, static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonClicked),
            this, &ComponentPreview::on_textIndependencePicked);

}

ComponentPreview::~ComponentPreview()
{
    delete ui;
}

void ComponentPreview::initialize()
{
    // Set callback for double clicking a component TODO: maybe move this somewhere else
    connect(circuitView, &CircuitView::componentSelected,
        this, &ComponentPreview::setEditExisting);
}

void ComponentPreview::initializeNewComponent()
{
    // Remove the last item from the scene
    if (component != nullptr && component->scene() == ui->preview->scene())
        ui->preview->scene()->removeItem(component);

    // If no component was edited before, initialize with default properties
    if (component == nullptr)
        component = new GraphicResistor(); //TODO generalize
    else
        component = new GraphicResistor(*(GraphicResistor*)component);

    // The component is not selectable nor movable inside the preview
    component->setFlag(QGraphicsItem::ItemIsMovable, false);
    component->setFlag(QGraphicsItem::ItemIsSelectable, false);

    // Update the contents of the window
    synchronize();
    ui->preview->scene()->addItem(component);
    ui->btnAdd->setVisible(true);
    ui->preview->setVisible(true);
}

void ComponentPreview::synchronize()
{
    ui->editAngle->setText(QString::number(component->rotation()));
    ui->editText->setText(component->getName());
    ui->editTextAngle->setText(QString::number(component->getTextRotation()));
    ui->btnDepend->setChecked(!component->isTextRotationIndependent());
    ui->btnIndependent->setChecked(component->isTextRotationIndependent());
}

void ComponentPreview::setCircuitView(CircuitView *cv)
{
    circuitView = cv;
}

void ComponentPreview::setEditExisting(GraphicComponent *component)
{
    // Remove the old component from the scene
    if (this->component->scene() == ui->preview->scene())
        ui->preview->scene()->removeItem(this->component);

    this->component = component;

    // Update the window
    synchronize();
    ui->btnAdd->setVisible(false);
    ui->preview->setVisible(false);
}

void ComponentPreview::on_textAnchorPicked(int id)
{
    component->setTextAnchor(GraphicComponent::Anchor(id));
    updatePreview();
}

void ComponentPreview::on_textIndependencePicked(int id)
{
    component->setTextRotationIndependent(id);
    updatePreview();
}

void ComponentPreview::on_btnAdd_clicked()
{
    circuitView->initiateInsertComponent(component, CircuitView::InsertPoints);
    initializeNewComponent();
}

void ComponentPreview::on_editAngle_textEdited(const QString &s)
{
    //TODO check for input validity in all similar methods
    component->setRotation(s.toFloat());
    updatePreview();
}

void ComponentPreview::on_editText_textEdited(const QString &s)
{
    component->setName(s);
    updatePreview();
}

void ComponentPreview::on_editTextAngle_textEdited(const QString &s)
{
    component->setTextRotation(s.toFloat());
    updatePreview();
}

void ComponentPreview::updatePreview()
{
    if (!component || !component->scene()) return;
    component->scene()->update();
}

}
