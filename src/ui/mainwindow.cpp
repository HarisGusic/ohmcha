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
    // Determine the directory to open
    QString defaultPath;

    QSettings settings("ohmcha", "ohmcha");
    QVariant value = settings.value("default_path");
    if (value.isNull())
        defaultPath = QDir::homePath();
    else
        defaultPath = value.toString();

    // Display the dialog and get the user choice
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", defaultPath);
    QFile file(fileName);

    if (!file.open(QFile::ReadWrite))
    {
        QMessageBox mb;
        mb.setText("Cannot open the selected file.");
        mb.exec();
        return;
    }

    // Save the current directory as the default one
    settings.setValue("default_path", QFileInfo(fileName).absolutePath());
    settings.sync();

    // Replace the old circuit view with a new one
    delete ui->circuitView;
    ui->circuitView = new CircuitView(this, xmlParseSchematic(fileName.toStdString()));
    ui->horizontalLayout->addWidget(ui->circuitView);
    ui->circuitView->initialize();
    ui->componentPreview->setCircuitView(ui->circuitView);
    ui->componentPreview->initialize();
}

}
