#include "edgecontroller.h"
#include "edgeitem.h"
#include "node.h"
#include <QGraphicsScene>

EdgeController::EdgeController(QGraphicsScene* scene)
    : scene(scene) {}

void EdgeController::addEdge(Node* a, Node* b, int weight)
{
    auto e = std::make_unique<EdgeItem>(a, b, weight);
    scene->addItem(e.get());
    edges.push_back(std::move(e));

    connect(a, &Node::moved, this, &EdgeController::updateEdges);
    connect(b, &Node::moved, this, &EdgeController::updateEdges);
}

void EdgeController::updateEdges()
{
    for (auto& e : edges)
        e->updatePosition();
}
