#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graph.h"
#include "createchanneldialog.h"
#include "removechanneldialog.h"
#include "ChannelProperties.h"
#include <QDialog>
#include <QMessageBox>
#include <QStandardItem>
#include <QGraphicsScene>
#include <math.h>
#include "messagesimulator.h"
#include "sendmessage.h"
#include "messagestatsdialog.h"

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

    edgeController = new EdgeController(scene, graph);
    connect(edgeController, &EdgeController::routingChanged,
            this, &MainWindow::updateRouting);

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
        int weight = randomWeight
                         ? Graph::ALLOWED_RANDOM_WEIGHTS[rand() % Graph::ALLOWED_RANDOM_WEIGHTS.size()]
                         : dlg.getWeight();
        ChannelType type = dlg.getChannelType();
        ChannelMode mode = dlg.getChannelMode();

        ChannelProperties props{ weight, type, mode };

        graph->connect(a, b, props);
        edgeController->addEdge(a, b, props);
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
    ui->labelTableTitle->setText(QString("Node %1").arg(node->getId()));

    ui->NodeON->blockSignals(true);
    ui->NodeOFF->blockSignals(true);
    ui->routerId->setNum(node->getId());
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

    if (graph->lsrStarted && !graph->lsrComplete)
        updateTopologyDB(node);
    else
        updateRouting();

    updateHighlight();
}

void MainWindow::on_NodeON_toggled(bool checked)
{
    if (currentNode && checked) {
        currentNode->setState(NodeState::ON);

        if (graph->lsrStarted && !graph->lsrComplete)
            updateTopologyDB(currentNode);
        else
            updateRouting();

        updateHighlight();
    }
}

void MainWindow::on_NodeOFF_toggled(bool checked)
{
    if (currentNode && checked) {
        currentNode->setState(NodeState::OFF);

        if (graph->lsrStarted && !graph->lsrComplete)
            updateTopologyDB(currentNode);
        else
            updateRouting();

        updateHighlight();
    }
}

void MainWindow::updateRouting()
{
    if (!currentNode) return;

    ui->labelTableTitle->setText("Routing Table (SPF Result)");

    auto table = graph->computeRoutingFrom(currentNode, currentMetric);

    QStandardItemModel* m = new QStandardItemModel(table.size(), 4, this);
    m->setHorizontalHeaderLabels({"Destination", "Next Hop", "Cost", "Hops"});

    std::vector<std::pair<Node*, RouteEntry>> vec(table.begin(), table.end());
    std::sort(vec.begin(), vec.end(),
              [](auto& a, auto& b){ return a.first->getId() < b.first->getId(); });

    int row = 0;
    for (auto& [dst, entry] : vec) {
        m->setItem(row, 0, new QStandardItem(QString::number(dst->getId())));
        m->setItem(row, 1, new QStandardItem(entry.nextHop ? QString::number(entry.nextHop->getId()) : "-"));
        m->setItem(row, 2, new QStandardItem(QString::number(entry.cost, 'f', 2)));
        m->setItem(row, 3, new QStandardItem(QString::number(entry.hops)));
        row++;
    }
    ui->tableView->setModel(m);
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

        graph->removeNode(node);
    }

    currentNode = nullptr;
    ui->labelTableTitle->setText("No node selected");
    ui->NodeON->setChecked(false);
    ui->NodeOFF->setChecked(false);
    ui->NodeON->setEnabled(false);
    ui->NodeOFF->setEnabled(false);
    ui->tableView->setModel(nullptr);
}

void MainWindow::on_actionGenerate_topology_triggered()
{
    constexpr int NODE_COUNT = 24;
    constexpr int AVERAGE_DEGREE = 4;
    constexpr int SATELLITE_CHANNELS = 2;
    const int TARGET_EDGES = (NODE_COUNT * AVERAGE_DEGREE) / 2;

    if (stepper) { delete stepper; stepper = nullptr; }
    if (edgeController) { delete edgeController; edgeController = nullptr; }
    if (graph) { delete graph; graph = nullptr; }
    if (selector) { delete selector; selector = nullptr; }

    QGraphicsScene* scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    scene->setSceneRect(0, 0, 800, 600);

    graph = new Graph(scene);
    selector = new NodeSelector(scene);
    edgeController = new EdgeController(scene, graph);

    connect(edgeController, &EdgeController::routingChanged,
            this, &MainWindow::updateRouting);
    connect(selector, &NodeSelector::oneNodeSelected,
            this, &MainWindow::onNodeSelected);

    std::vector<Node*> nodes;
    nodes.reserve(NODE_COUNT);

    const double centerX = 400.0, centerY = 300.0, radius = 250.0;
    for (int i = 0; i < NODE_COUNT; ++i) {
        double angle = (2 * M_PI * i) / NODE_COUNT;
        QPointF pos(centerX + radius * std::cos(angle),
                    centerY + radius * std::sin(angle));
        Node* node = graph->add_node(std::make_unique<Node>(i + 1, pos));
        nodes.push_back(node);
        edgeController->watchNode(node);
    }

    int edgeCount = 0;
    auto randomWeight = []() {
        return Graph::ALLOWED_RANDOM_WEIGHTS[
            rand() % Graph::ALLOWED_RANDOM_WEIGHTS.size()];
    };

    for (int i = 1; i < NODE_COUNT; ++i) {
        Node* a = nodes[i];
        Node* b = nodes[i - 1];
        ChannelProperties props{ randomWeight(), ChannelType::Duplex, ChannelMode::Normal };
        graph->connect(a, b, props);
        edgeController->addEdge(a, b, props);
        edgeCount++;
    }

    while (edgeCount < TARGET_EDGES - SATELLITE_CHANNELS) {
        Node* a = nodes[rand() % NODE_COUNT];
        Node* b = nodes[rand() % NODE_COUNT];
        if (a == b || edgeController->findEdge(a, b)) continue;
        ChannelProperties props{ randomWeight(), ChannelType::Duplex, ChannelMode::Normal };
        graph->connect(a, b, props);
        edgeController->addEdge(a, b, props);
        edgeCount++;
    }

    int sat = 0;
    while (sat < SATELLITE_CHANNELS) {
        Node* a = nodes[rand() % NODE_COUNT];
        Node* b = nodes[rand() % NODE_COUNT];
        if (a == b || edgeController->findEdge(a, b)) continue;
        ChannelProperties props{ randomWeight(), ChannelType::Duplex, ChannelMode::Satellite };
        graph->connect(a, b, props);
        edgeController->addEdge(a, b, props);
        sat++;
        edgeCount++;
    }

    graph->applyForceDirectedLayout();

    graph->invalidateLSR();
    currentNode = nullptr;
    currentMetric = RouteMetric::EffectiveCost;

    ui->labelTableTitle->setText("No node selected");
    ui->NodeON->setChecked(false);
    ui->NodeOFF->setChecked(false);
    ui->NodeON->setEnabled(false);
    ui->NodeOFF->setEnabled(false);
    ui->tableView->setModel(nullptr);

    double avgDegree = static_cast<double>(2 * edgeCount) / NODE_COUNT;
    QMessageBox::information(this, "Topology generated",
                             QString("Created topology:\n"
                                     "Nodes: %1\nEdges: %2\n"
                                     "Average degree: %3\nSatellite channels: %4")
                                 .arg(NODE_COUNT)
                                 .arg(edgeCount)
                                 .arg(avgDegree, 0, 'f', 2)
                                 .arg(SATELLITE_CHANNELS));
}

void MainWindow::on_actionArrange_Graph_triggered()
{
    graph->applyForceDirectedLayout();
}

void MainWindow::on_actionRouting_Next_triggered()
{
    if (!stepper || stepper->finished()) return;

    DijkstraStep step;
    if (stepper->step(step)) {
        edgeController->highlightRelaxations(step.relaxedEdges);
        updateRouting();
    } else {
        QMessageBox::information(this, "SPF", "Algorithm is finished");
    }
}

void MainWindow::on_actionRouting_Start_triggered()
{
    if (!currentNode) return;

    routingSource = currentNode;
    currentMetric = RouteMetric::EffectiveCost;
    stepper = new DijkstraStepper(graph, routingSource, currentMetric);

    edgeController->clearHighlight();
    QMessageBox::information(this, "SPF", "Dijkstra started from selected node.");
}

void MainWindow::updateTopologyDB(Node* r) {
    if (!r) return;

    ui->labelTableTitle->setText("Topology DB (LSR Phase)");

    auto& db = r->topologyTable;

    QStandardItemModel* m = new QStandardItemModel(this);
    m->setHorizontalHeaderLabels({"Router", "Neighbor", "Cost"});

    for(auto &entr : db) {
        QList<QStandardItem*> row;
        row << new QStandardItem(QString::number(entr.from->getId()));
        row << new QStandardItem(QString::number(entr.to->getId()));
        row << new QStandardItem(QString::number(entr.props.weight));
        m->appendRow(row);
    }

    ui->tableView->setModel(m);

    updateHighlight();
}

void MainWindow::on_actionRouting_Reset_triggered()
{
    edgeController->clearHighlight();
}

void MainWindow::on_actionLSR_Run_triggered()
{
    if(!graph->lsrStarted || graph->lsrComplete){
        graph->LSRInit();
    }
    while(graph->performLSAStep()) {}
    QMessageBox::information(this, "LSR", "LSR Completed. All routers now share the same topology database.");

    if (currentNode)
        updateTopologyDB(currentNode);
    updateHighlight();
}

void MainWindow::on_actionLSR_Start_triggered()
{
    if (graph->lsrStarted && !graph->lsrComplete) {
        QMessageBox::warning(this, "LSR", "Already running.");
        return;
    } else if (graph->lsrComplete) {
        graph->invalidateLSR();
    }

    graph->LSRInit();
    updateTopologyDB(currentNode);
    updateHighlight();
}

void MainWindow::on_actionLSR_Next_Step_triggered()
{
    if (!graph->lsrStarted || graph->lsrComplete) return;

    if (graph->performLSAStep()) {
        updateTopologyDB(currentNode);
    } else {
        QMessageBox::information(this, "LSR", "Topology synchronized.");
    }
    updateHighlight();
}

void MainWindow::on_actionLSR_Reset_2_triggered()
{
    graph->invalidateLSR();
    updateTopologyDB(currentNode);
    updateHighlight();
}

void MainWindow::updateHighlight()
{
    if (!currentNode) {
        edgeController->clearHighlight();
        return;
    }

    if (graph->lsrStarted && !graph->lsrComplete) {
        edgeController->highlightKnownFor(currentNode);
    } else {
        edgeController->clearHighlight();
    }
}

void MainWindow::on_actionSend_Message_triggered()
{
    auto selected = ui->graphicsView->scene()->selectedItems();
    SendMessage dlg(graph->getAllNodes(), this);

    if (selected.size() == 2) {
        Node* a = qgraphicsitem_cast<Node*>(selected[0]);
        Node* b = qgraphicsitem_cast<Node*>(selected[1]);
        dlg.presetNodes(a, b);
    }

    if (dlg.exec() != QDialog::Accepted)
        return;

    Node* src = dlg.getSource();
    Node* dst = dlg.getDestination();
    int msgSize = dlg.getMessageSizeBytes();
    int pkt = dlg.getPacketSizeBytes();
    bool datagram = dlg.isDatagramMode();

    MessageSimulationResult res = datagram
                                      ? MessageSimulator::sendDatagram(graph, src, dst, msgSize, pkt)
                                      : MessageSimulator::sendVirtualCircuit(graph, src, dst, msgSize, pkt);

    if (res.lastPath.size() >= 2)
        edgeController->highlightPath(res.lastPath);
    else
        edgeController->clearHighlight();

    QMessageBox::information(this, "Simulation Result",
                             QString("Mode: %1\nTotal time: %2 ms\nPackets: %3\nService data: %4 bytes")
                                 .arg(datagram ? "Datagram" : "Virtual Circuit")
                                 .arg(res.totalTime, 0, 'f', 2)
                                 .arg(res.totalPackets)
                                 .arg(res.serviceDataSize));

    MessageLogEntry log;
    log.srcId = src->getId();
    log.dstId = dst->getId();
    log.mode = datagram ? "Datagram" : "Virtual Circuit";
    log.messageSize = msgSize;
    log.packetSize = pkt;
    log.totalPackets = res.totalPackets;
    log.serviceBytes = res.serviceDataSize;
    log.totalTime = res.totalTime;

    QStringList nodes;
    for (Node* n : res.lastPath)
        nodes << QString::number(n->getId());
    log.pathString = nodes.join(" -> ");

    messageLog.push_back(log);
}

void MainWindow::on_actionView_Logs_triggered()
{
    MessageStatsDialog dlg(messageLog, this);
    dlg.exec();
}


void MainWindow::on_actionClear_triggered()
{
    if (edgeController) {
        disconnect(edgeController, &EdgeController::routingChanged,
                   this, &MainWindow::updateRouting);
    }

    if (selector) {
        disconnect(selector, &NodeSelector::oneNodeSelected,
                   this, &MainWindow::onNodeSelected);
    }

    currentNode = nullptr;

    if (stepper) {
        delete stepper;
        stepper = nullptr;
    }

    if (edgeController) {
        delete edgeController;
        edgeController = nullptr;
    }

    if (selector) {
        delete selector;
        selector = nullptr;
    }

    QGraphicsScene* oldScene = ui->graphicsView->scene();

    if (graph) {
        delete graph;
        graph = nullptr;
    }

    if (oldScene) {
        delete oldScene;
    }

    QGraphicsScene* scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    scene->setSceneRect(0, 0, 800, 600);

    graph = new Graph(scene);
    selector = new NodeSelector(scene);
    edgeController = new EdgeController(scene, graph);

    connect(edgeController, &EdgeController::routingChanged,
            this, &MainWindow::updateRouting);
    connect(selector, &NodeSelector::oneNodeSelected,
            this, &MainWindow::onNodeSelected);

    currentMetric = RouteMetric::EffectiveCost;
    ui->labelTableTitle->setText("No node selected");
    ui->NodeON->setChecked(false);
    ui->NodeOFF->setChecked(false);
    ui->NodeON->setEnabled(false);
    ui->NodeOFF->setEnabled(false);
    ui->tableView->setModel(nullptr);

    messageLog.clear();

    QMessageBox::information(this, "Clear", "Scene cleared.");
}



