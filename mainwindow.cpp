#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graph.h"
#include "createchanneldialog.h"
#include <QDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QGraphicsScene *scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    graph = new Graph(scene);

    ui->graphicsView->setDragMode(QGraphicsView::RubberBandDrag);

    selector = new NodeSelector(scene);

    edgeController = new EdgeController(scene);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionZoom_In_triggered()
{
    constexpr double factor = 1.2;
    ui->graphicsView->scale(factor, factor);
}

void MainWindow::on_actionZoom_Out_triggered()
{
    constexpr double factor = 1/1.2;
    ui->graphicsView->scale(factor, factor);
}

void MainWindow::on_actionAddNode_triggered()
{
    QPointF scenePos = ui->graphicsView->mapToScene(
        ui->graphicsView->viewport()->rect().center());

    static int nextId = 1;
    auto node = std::make_unique<Node>(nextId++, scenePos);
    Node* node_ptr = graph->add_node(std::move(node));

    auto selected = ui->graphicsView->scene()->selectedItems();
    if (selected.size() >= 2)
        selected[0]->setSelected(false);

    node_ptr->setSelected(true);
}



void MainWindow::on_actionAddConnection_triggered()
{
    CreateChannelDialog dlg(this);

    dlg.setNodesList(graph->getAllNodes());

    auto selected = ui->graphicsView->scene()->selectedItems();
    if (selected.size() >= 1)
        dlg.presetNodes(qgraphicsitem_cast<Node*>(selected[0]), nullptr);
    if (selected.size() >= 2)
        dlg.presetNodes(qgraphicsitem_cast<Node*>(selected[0]),
                        qgraphicsitem_cast<Node*>(selected[1]));

    if(dlg.exec() == QDialog::Accepted) {
        Node* a = dlg.getNodeA();
        Node* b = dlg.getNodeB();
        bool duplex = dlg.isDuplex();
        bool randomWeight = dlg.isRandomWeight();
        int weight = 0;
        if(randomWeight) {
            srand(time(0));
            weight = rand();
        } else {
            weight = dlg.getWeight();
        }
        graph->connect(a, b, duplex, weight);
        edgeController->addEdge(a, b, weight);
    }
}


