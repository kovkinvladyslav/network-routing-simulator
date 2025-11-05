#include "edgecontroller.h"
#include "edgeitem.h"
#include "node.h"
#include <QGraphicsScene>

EdgeController::EdgeController(QGraphicsScene* scene)
    : scene(scene) {}

void EdgeController::addEdge(Node* a, Node* b, int weight, ChannelType type, ChannelMode mode)
{
    auto e = std::make_unique<EdgeItem>(a, b, weight, type, mode);

    connect(e.get(), &EdgeItem::requestRemove,
            this, &EdgeController::removeEdge);

    scene->addItem(e.get());
    edges.push_back(std::move(e));
}
#include <algorithm>

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

    scene->removeItem(edge);

    edges.erase(std::remove_if(edges.begin(), edges.end(),
                               [&](const std::unique_ptr<EdgeItem>& p){ return p.get() == edge; }),
                edges.end());
}

void EdgeController::updateEdges()
{
    for (auto& e : edges)
        e->updatePosition();
}

