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

    // Constructors
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Getters
    /**
     * @return The currently displayed Circuit View.
     */
    CircuitView *getCircuitView();

private slots:

    void on_listComponents_itemDoubleClicked(QListWidgetItem *item);
    void on_actionOpen_triggered();
    void on_actionNew_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();

    void on_actionSolve_triggered();

    void on_actionHideSolution_triggered();

private:

    // Private methods
    /** Refers to the component list in the docked widget. */
    void initializeComponentList();

    // Attributes

    Ui::MainWindow *ui;
    QString fileName;

};

}
#endif // MAINWINDOW_H
