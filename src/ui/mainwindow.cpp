#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "circuitview.h"

namespace Ohmcha
{

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initializeComponentList();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionInsertComponent_triggered()
{
    //TODO change
    ui->circuitView->insertComponent(nullptr, Ohmcha::CircuitView::InsertPoints);
}

void MainWindow::initializeComponentList()
{
    new QListWidgetItem("Resistor", ui->listComponents);
}

void MainWindow::on_listComponents_itemDoubleClicked(QListWidgetItem *item)
{
    ui->dockInsert->setVisible(true);
    //ui->dockInsert->setFloating(!ui->dockInsert->isFloating());
    //ui->dockInsert->setEnabled(true);
}

}
