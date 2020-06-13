#include "mainwindow.h"
#include "ui_mainwindow.h"

namespace Ohmcha
{

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initializeComponentList();

    ui->componentPreview->setCircuitView(ui->circuitView);
    ui->componentPreview->initialize();
    ui->componentPreview->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

CircuitView *MainWindow::getCircuitView()
{
    return ui->circuitView;
}

void MainWindow::on_listComponents_itemDoubleClicked(QListWidgetItem *item)
{
    ui->dockInsert->setVisible(true);
    ui->componentPreview->initializeNewComponent();
    ui->componentPreview->setVisible(true);
}

void MainWindow::initializeComponentList()
{
    new QListWidgetItem("Resistor", ui->listComponents);
}

}
