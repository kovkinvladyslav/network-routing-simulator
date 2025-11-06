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

void EdgeController::clearHighlight()
{
    for (auto &e : edges) {
        QPen p = e->pen();
        p.setWidth(2);
        p.setColor(Qt::white);
        e->setPen(p);
    }
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


