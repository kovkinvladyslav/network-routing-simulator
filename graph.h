#ifndef GRAPH_H
#define GRAPH_H
#include "node.h"
#include <vector>
#include <QGraphicsScene>
#include <memory>
#include <QPointF>
#include <unordered_set>

class Graph
{
public:
    Graph(QGraphicsScene* scene);
    Node* add_node(std::unique_ptr<Node> node);
    std::vector<Node*> getAllNodes() const;
    void connect(Node* nodeA, Node* nodeB, bool isDuplex, int weight);
private:
    QGraphicsScene* scene;
    std::vector<std::unique_ptr<Node>> nodes;
    std::unordered_set<Node*> nodes_search;
};

#endif // GRAPH_H
