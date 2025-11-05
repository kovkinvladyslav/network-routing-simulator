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
    void connect(Node* nodeA, Node* nodeB, ChannelType type, ChannelMode mode, int weight);
    double computeTransmissionTime(const ChannelProperties& ch, int packetBytes);
    void removeConnection(Node* a, Node* b);
    void removeNode(Node* node);


private:
    QGraphicsScene* scene;
    std::vector<std::unique_ptr<Node>> nodes;
    std::unordered_set<Node*> nodes_search;
};

#endif // GRAPH_H
