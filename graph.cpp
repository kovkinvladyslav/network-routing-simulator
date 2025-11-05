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

void Graph::connect(Node* nodeA, Node* nodeB, ChannelType type, ChannelMode mode, int weight)
{
    nodeA->add_adj(nodeB, weight, type, mode);
    nodeB->add_adj(nodeA, weight, type, mode);
}
constexpr double SATELLITE_LATENCY = 0.25;

double Graph::computeTransmissionTime(const ChannelProperties &ch, int packetBytes)
{
    double t = (packetBytes / 100.0) * ch.weight;

    if (ch.type == ChannelType::HalfDuplex)
        t *= 2;

    if (ch.mode == ChannelMode::Satellite)
        t += SATELLITE_LATENCY;

    return t;
}

void Graph::removeConnection(Node* a, Node* b)
{
    if (!a || !b) return;

    a->removeAdj(b);
    b->removeAdj(a);
}




