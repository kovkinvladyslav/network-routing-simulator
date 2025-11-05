#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graph.h"
#include <QMainWindow>
#include "nodeselector.h"
#include "edgecontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionZoom_In_triggered();

    void on_actionZoom_Out_triggered();

    void on_actionAddNode_triggered();

    void on_actionAddConnection_triggered();

    void on_actionDelete_Connection_triggered();

private:
    Ui::MainWindow *ui;
    Graph* graph;
    NodeSelector *selector;
    EdgeController *edgeController;
};
#endif // MAINWINDOW_H
