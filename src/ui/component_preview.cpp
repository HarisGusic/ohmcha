#include "component_preview.h"
#include "ui_component_preview.h"

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

}
