#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "src/fileio/xml.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

namespace Ohmcha
{

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initializeComponentList();

    ui->circuitView->initialize();
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
    ui->circuitView->scene()->clearSelection();
    ui->componentPreview->setComponentType(item->text());
    ui->componentPreview->initializeNewComponent(item->text());
    ui->componentPreview->setVisible(true);
}

void MainWindow::initializeComponentList()
{
    new QListWidgetItem("Resistor", ui->listComponents);
    new QListWidgetItem("Emf", ui->listComponents);
    new QListWidgetItem("Current Source", ui->listComponents);
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", QDir::homePath());
    QFile file(fileName);

    if (!file.open(QFile::ReadWrite))
    {
        QMessageBox mb;
        mb.setText("Cannot open the selected file.");
        mb.exec();
        return;
    }

    delete ui->circuitView;
    ui->circuitView = new CircuitView(this, xmlParseSchematic(fileName.toStdString()));
    ui->horizontalLayout->addWidget(ui->circuitView);
    ui->circuitView->initialize();
    ui->componentPreview->setCircuitView(ui->circuitView);
    ui->componentPreview->initialize();
}

}
