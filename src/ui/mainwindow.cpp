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

QString getDefaultPath()
{
    // Determine the directory to open
    QString defaultPath;

    QSettings settings("ohmcha", "ohmcha");
    QVariant value = settings.value("default_path");
    if (value.isNull())
        defaultPath = QDir::homePath();
    else
        defaultPath = value.toString();

    return defaultPath;
}

void MainWindow::on_actionOpen_triggered()
{
    auto defaultPath = getDefaultPath();
    // Display the dialog and get the user choice
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", defaultPath);
    QFile file(fileName);

    if (fileName == "")
        return;

    if (!file.open(QFile::ReadWrite))
    {
        QMessageBox mb;
        mb.setText("Cannot open the selected file.");
        mb.exec();
        return;
    }

    this->fileName = fileName;

    // Save the current directory as the default one
    QSettings settings("ohmcha", "ohmcha");
    settings.setValue("default_path", QFileInfo(fileName).absolutePath());
    settings.sync();

    // Replace the old circuit view with a new one
    delete ui->circuitView;
    try {
        ui->circuitView = new CircuitView(this, xmlParseSchematic(fileName.toStdString()));
    } catch (std::exception &e) {
        QMessageBox mb;
        mb.setText("Selected file is invalid");
        mb.exec();
        return;
    }
    ui->horizontalLayout->addWidget(ui->circuitView);
    ui->circuitView->initialize();
    ui->componentPreview->setCircuitView(ui->circuitView);
    ui->componentPreview->initialize();

    file.close();
}

void MainWindow::on_actionNew_triggered()
{
    delete ui->circuitView;
    ui->circuitView = new CircuitView(this);
    ui->horizontalLayout->addWidget(ui->circuitView);
    ui->circuitView->initialize();
    ui->componentPreview->setCircuitView(ui->circuitView);
    ui->componentPreview->initialize();
    fileName = "";
}

void saveToFile(Ui::MainWindow *ui, QString fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly))
    {
        QMessageBox mb;
        mb.setText("Unable to save changes.");
        mb.exec();
        return;
    }
    auto *schematic = ui->circuitView->getSchematic();
    file.write(xmlConvertSchematic(schematic).toUtf8());
    file.close();
}

void MainWindow::on_actionSave_triggered()
{
    if (fileName == "")
        return on_actionSaveAs_triggered();
    saveToFile(ui, fileName);
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString defaultPath;
    if (fileName == "")
        defaultPath = getDefaultPath();
    else
        defaultPath = QFileInfo(this->fileName).absolutePath();

    QString fileName = QFileDialog::getSaveFileName(this, "Save to", defaultPath);
    if (fileName == "")
        return;

    saveToFile(ui, fileName);

    this->fileName = fileName;
}

void MainWindow::on_actionSolve_triggered()
{
    if (ui->circuitView)
    {
        Schematic *schematic = new Schematic(*ui->circuitView->getSchematic());
        auto rect = ui->circuitView->sceneRect();
        auto zoom = ui->circuitView->getZoomLevel();
        delete ui->circuitView;
        ui->circuitView = new CircuitView(this, schematic);
        ui->horizontalLayout->addWidget(ui->circuitView);
        ui->circuitView->initialize();
        ui->componentPreview->setCircuitView(ui->circuitView);
        ui->componentPreview->initialize();
        ui->circuitView->setSceneRect(rect);
        ui->circuitView->setZoomLevel(zoom);

        ui->circuitView->solve();
    }
}

}

void Ohmcha::MainWindow::on_actionHideSolution_triggered()
{
    ui->circuitView->showValues(false);
}
