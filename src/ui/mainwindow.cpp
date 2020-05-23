#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "circuitview.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionInsertComponent_triggered()
{
    //TODO change
    ui->circuitView->insertComponent(nullptr, CircuitView::InsertPoints);
}
