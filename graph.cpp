#include "graph.h"

Graph::Graph(QGraphicsScene *scene)
    :scene(scene){
}

Node* Graph::add_node(int id, QPointF pos)
{
    auto node = std::make_unique<Node>(id, pos);
    Node* node_ptr = node.get();
    nodes.push_back(std::move(node));
    nodes_search.insert(node_ptr);
    scene->addItem(node_ptr);
    return node_ptr;
}
