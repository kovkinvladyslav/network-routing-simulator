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
    void node_clicked(Node *node);
    void clear_selection();
private:
    QGraphicsScene* scene;
    std::vector<std::unique_ptr<Node>> nodes;
    std::unordered_set<Node*> nodes_search;
    Node* first_selected = nullptr;
    Node* second_selected = nullptr;
};

#endif // GRAPH_H
