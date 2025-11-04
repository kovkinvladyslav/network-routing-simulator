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
    Node* add_node(int id, QPointF pos);
private:
    QGraphicsScene* scene;
    std::vector<std::unique_ptr<Node>> nodes;
    std::unordered_set<Node*> nodes_search;
};

#endif // GRAPH_H
