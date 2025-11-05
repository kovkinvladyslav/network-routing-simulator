#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graph.h"
#include "createchanneldialog.h"
#include <QDialog>
#include <QMessageBox>
#include <QStandardItem>
#include "removechanneldialog.h"

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
    connect(selector, &NodeSelector::oneNodeSelected,
            this, &MainWindow::onNodeSelected);
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

    edgeController->watchNode(node_ptr);

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

    if (dlg.exec() == QDialog::Accepted) {
        Node* a = dlg.getNodeA();
        Node* b = dlg.getNodeB();
        bool randomWeight = dlg.isRandomWeight();
        int weight = randomWeight ? rand() : dlg.getWeight();
        ChannelType type = dlg.getChannelType();
        ChannelMode mode = dlg.getChannelMode();

        graph->connect(a, b, type, mode, weight);
        edgeController->addEdge(a, b, weight, type, mode);
    }
}

void MainWindow::on_actionDelete_Connection_triggered()
{
    RemoveChannelDialog dlg(this);
    dlg.setNodesList(graph->getAllNodes());

    auto selected = ui->graphicsView->scene()->selectedItems();
    if (selected.size() >= 1)
        dlg.presetNodes(qgraphicsitem_cast<Node*>(selected[0]), nullptr);
    if (selected.size() >= 2)
        dlg.presetNodes(qgraphicsitem_cast<Node*>(selected[0]),
                        qgraphicsitem_cast<Node*>(selected[1]));

    if (dlg.exec() == QDialog::Accepted) {
        Node* a = dlg.getNodeA();
        Node* b = dlg.getNodeB();

        auto& adj = a->get_adj();
        if (!adj.count(b)) {
            QMessageBox::warning(this, "No Connection", "These nodes are not connected.");
            return;
        }

        graph->removeConnection(a, b);

        EdgeItem* e = edgeController->findEdge(a, b);
        edgeController->removeEdge(e);

        onNodeSelected(a);
        onNodeSelected(b);
    }
}

void MainWindow::onNodeSelected(Node* node)
{
    currentNode = node;
    ui->label->setText(QString("Node %1").arg(node->getId()));

    ui->NodeON->blockSignals(true);
    ui->NodeOFF->blockSignals(true);

    bool hasConnections = !node->get_adj().empty();

    if (!hasConnections) {
        currentNode->setState(NodeState::DISCONNECTED);
        ui->NodeON->setChecked(false);
        ui->NodeOFF->setChecked(false);
        ui->NodeON->setEnabled(false);
        ui->NodeOFF->setEnabled(false);
    } else {
        ui->NodeON->setEnabled(true);
        ui->NodeOFF->setEnabled(true);

        if (node->getState() == NodeState::ON) {
            ui->NodeON->setChecked(true);
            ui->NodeOFF->setChecked(false);
        } else {
            ui->NodeON->setChecked(false);
            ui->NodeOFF->setChecked(true);
        }
    }

    ui->NodeON->blockSignals(false);
    ui->NodeOFF->blockSignals(false);

    auto& adj = node->get_adj();
    QStandardItemModel* model = new QStandardItemModel(adj.size(), 4, this);
    model->setHorizontalHeaderLabels({"Neighbor", "Weight", "Type", "Mode"});

    int row = 0;
    for (auto& [nbr, props] : adj) {
        model->setItem(row, 0, new QStandardItem(QString::number(nbr->getId())));
        model->setItem(row, 1, new QStandardItem(QString::number(props.weight)));
        model->setItem(row, 2, new QStandardItem(props.type == ChannelType::Duplex ? "Duplex" : "Half-Duplex"));
        model->setItem(row, 3, new QStandardItem(props.mode == ChannelMode::Satellite ? "Satellite" : "Normal"));
        row++;
    }

    ui->tableView->setModel(model);
}

void MainWindow::on_NodeON_toggled(bool checked)
{
    if (currentNode && checked)
        currentNode->setState(NodeState::ON);
}

void MainWindow::on_NodeOFF_toggled(bool checked)
{
    if (currentNode && checked)
        currentNode->setState(NodeState::OFF);
}

void MainWindow::on_actionDelete_Node_triggered()
{
    auto selected = ui->graphicsView->scene()->selectedItems();
    if (selected.empty()) {
        QMessageBox::information(this, "Delete Node", "Select at least one node to delete.");
        return;
    }

    std::vector<Node*> nodesToDelete;
    nodesToDelete.reserve(selected.size());
    for (int i = 0; i < selected.size(); ++i) {
        if (auto node = qgraphicsitem_cast<Node*>(selected[i]))
            nodesToDelete.push_back(node);
    }

    for (Node* node : nodesToDelete) {

        std::vector<Node*> neighbors;
        neighbors.reserve(node->get_adj().size());
        for (auto& [nbr, props] : node->get_adj())
            neighbors.push_back(nbr);

        for (Node* nbr : neighbors) {
            graph->removeConnection(node, nbr);
            if (auto e = edgeController->findEdge(node, nbr))
                edgeController->removeEdge(e);
            onNodeSelected(nbr);
        }

        ui->graphicsView->scene()->removeItem(node);
        delete node;
    }

    currentNode = nullptr;
    ui->label->setText("No node selected");
    ui->NodeON->setChecked(false);
    ui->NodeOFF->setChecked(false);
    ui->NodeON->setEnabled(false);
    ui->NodeOFF->setEnabled(false);
    ui->tableView->setModel(nullptr);
}
