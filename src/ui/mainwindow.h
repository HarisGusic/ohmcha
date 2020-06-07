#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "circuitview.h"

#include <QMainWindow>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

namespace Ohmcha
{

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    CircuitView *getCircuitView();

private slots:
    void on_actionInsertComponent_triggered();

    void on_listComponents_itemDoubleClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;

    void initializeComponentList();
};

}
#endif // MAINWINDOW_H
