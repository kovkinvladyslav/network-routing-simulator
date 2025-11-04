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

void Graph::node_clicked(Node *node)
{
    if(!first_selected) {
        first_selected = node;
        node->hightlight(true);
    }
    if(first_selected && !second_selected && node != first_selected) {
        second_selected = node;
        node->hightlight(true);
    }
}

void Graph::clear_selection()
{
    if(first_selected) {
        first_selected->hightlight(false);
        first_selected = nullptr;
    }
    if(second_selected) {
        second_selected->hightlight(false);
        second_selected = nullptr;
    }
}
