// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graph.h"
#include "nodeselector.h"
#include "edgecontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
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
    void on_actionDelete_Node_triggered();
    void onNodeSelected(Node* node);
    void on_NodeON_toggled(bool checked);
    void on_NodeOFF_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    Graph* graph;
    NodeSelector* selector;
    EdgeController* edgeController;
    Node* currentNode = nullptr;

    void updateInspector(Node* node);
    void resetInspector();
    void removeEdgeBetween(Node* a, Node* b);
    void removeAllEdges(Node* node);
    QVector<Node*> selectedNodes() const;
};

#endif // MAINWINDOW_H
