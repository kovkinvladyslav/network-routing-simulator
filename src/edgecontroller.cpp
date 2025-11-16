#include "edgecontroller.h"
#include "edgeitem.h"
#include "node.h"
#include <QGraphicsScene>
#include <algorithm>

EdgeController::EdgeController(QGraphicsScene* scene, Graph* graph)
    : scene(scene), graph(graph) {}


void EdgeController::addEdge(Node* a, Node* b, const ChannelProperties& props)
{
    auto e = std::make_unique<EdgeItem>(a, b, props);
    connect(e.get(), &EdgeItem::requestRemove,
            this, &EdgeController::removeEdge);
    connect(e.get(), &EdgeItem::channelStateChanged, this, &EdgeController::updateEdges);

    scene->addItem(e.get());
    edges.push_back(std::move(e));
}

EdgeItem* EdgeController::findEdge(Node* a, Node* b)
{
    for (auto &e : edges)
        if ((e->a == a && e->b == b) || (e->a == b && e->b == a))
            return e.get();
    return nullptr;
}

void EdgeController::watchNode(Node* node)
{
    connect(node, &Node::moved, this, &EdgeController::updateEdges);
}
void EdgeController::removeEdge(EdgeItem* edge)
{
    if (!edge) return;

    graph->removeConnection(edge->a, edge->b);

    scene->removeItem(edge);

    edges.erase(std::remove_if(edges.begin(), edges.end(),
                               [&](const std::unique_ptr<EdgeItem>& p){ return p.get() == edge; }),
                edges.end());

    emit routingChanged();
}

void EdgeController::updateEdges()
{
    for (auto& e : edges)
        e->updatePosition();
    emit routingChanged();

}

void EdgeController::highlightPath(const std::vector<Node*>& path)
{
    clearHighlight();
    for (int i = 0; i + 1 < (int)path.size(); ++i) {
        Node* u = path[i];
        Node* v = path[i+1];
        if (auto e = findEdge(u, v)) {
            QPen p = e->pen();
            p.setWidth(4);
            p.setColor(Qt::yellow);
            e->setPen(p);
        }
    }
}

void EdgeController::highlightRelaxations(const std::vector<std::pair<Node*,Node*>>& relaxEdges)
{
    clearHighlight();
    for (auto& [u, v] : relaxEdges) {
        if (auto e = findEdge(u, v)) {
            QPen p = e->pen();
            p.setWidth(4);
            p.setColor(Qt::cyan);
            e->setPen(p);
        }
    }
}

void EdgeController::highlightKnownFor(Node *r)
{
    clearHighlight();
    if (!r) return;

    for (const auto& eInfo : r->topologyTable) {
        if (auto e = findEdge(eInfo.from, eInfo.to)) {
            QPen p = e->pen();
            p.setWidth(4);
            p.setColor(Qt::yellow);
            e->setPen(p);
        }
    }
}


void EdgeController::highlightSourceNode(Node* source)
{
    clearHighlight();
    if (!source) return;

    source->highlight(true);
    source->setBrush(QBrush(QColor(0, 200, 0)));
}

void EdgeController::highlightDijkstraStep(const DijkstraStep& step, DijkstraStepper* stepper)
{
    clearHighlight();

    Node* source = stepper->getSource();
    const auto& parents = stepper->getParents();
    const auto& queue = stepper->getQueue();

    source->setBrush(QBrush(QColor(0, 200, 0)));

    if (step.settled) {
        step.settled->setBrush(QBrush(QColor(255, 165, 0)));
    }

    if (step.nextToSettle && step.nextToSettle != step.settled) {
        step.nextToSettle->setBrush(QBrush(QColor(255, 255, 0)));
    }

    for (auto& [node, parent] : parents) {
        if (parent && node != source) {
            bool isSettled = std::find(queue.begin(), queue.end(), node) == queue.end();

            if (auto e = findEdge(node, parent)) {
                QPen p = e->pen();
                p.setWidth(3);
                p.setColor(isSettled ? QColor(0, 255, 0) : QColor(100, 255, 100));
                e->setPen(p);
            }
        }
    }

    for (auto& [u, v] : step.relaxedEdges) {
        if (auto e = findEdge(u, v)) {
            QPen p = e->pen();
            p.setWidth(5);
            p.setColor(QColor(0, 255, 255));
            e->setPen(p);
        }
    }

    for (auto& [node, parent] : parents) {
        bool isSettled = std::find(queue.begin(), queue.end(), node) == queue.end();
        if (isSettled && node != source && node != step.settled) {
            node->setBrush(QBrush(QColor(100, 150, 255)));
        }
    }

    for (Node* node : queue) {
        if (node != step.nextToSettle) {
        }
    }
}

void EdgeController::highlightFinalTree(DijkstraStepper* stepper)
{
    clearHighlight();

    Node* source = stepper->getSource();
    const auto& parents = stepper->getParents();

    source->setBrush(QBrush(QColor(0, 200, 0)));

    for (auto& [node, parent] : parents) {
        if (parent && node != source) {
            if (auto e = findEdge(node, parent)) {
                QPen p = e->pen();
                p.setWidth(4);
                p.setColor(QColor(0, 255, 0));
                e->setPen(p);
            }
        }
    }

    for (auto& [node, parent] : parents) {
        if (node != source) {
            node->setBrush(QBrush(QColor(100, 200, 255)));
        }
    }
}

void EdgeController::refreshAllEdges()
{
    for (auto& e : edges) {
        EdgeItem* edge = e.get();
        const auto& props = edge->a->get_adj().at(edge->b);
        edge->updateStyle(props);
    }
}


void EdgeController::clearHighlight()
{
    for (auto &e : edges) {
        QPen p = e->pen();
        p.setWidth(2);
        p.setColor(Qt::white);
        e->setPen(p);
    }

    for (auto& node : graph->getAllNodes()) {
        node->highlight(false);
        switch(node->getState()) {
        case NodeState::ON:
            node->setBrush(Qt::green);
            break;
        case NodeState::OFF:
            node->setBrush(Qt::red);
            break;
        case NodeState::DISCONNECTED:
            node->setBrush(Qt::gray);
            break;
        }
    }
}


