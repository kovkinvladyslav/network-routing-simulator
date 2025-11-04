#include "graph.h"

Graph::Graph(QGraphicsScene *scene)
    :scene(scene){
}

Node* Graph::add_node(std::unique_ptr<Node> node)
{
    Node* node_ptr = node.get();
    nodes_search.insert(node_ptr);
    scene->addItem(node_ptr);
    nodes.push_back(std::move(node));
    return node_ptr;
}

std::vector<Node *> Graph::getAllNodes() const
{
    std::vector<Node*> result;
    result.reserve(nodes.size());
    for (auto &ptr : nodes)
        result.push_back(ptr.get());
    return result;
}

void Graph::connect(Node *nodeA, Node *nodeB, bool isDuplex, int weight)
{
    nodeA->add_adj(nodeB, weight);
}

