#ifndef GRAPH_H
#define GRAPH_H
#include "node.h"
#include <vector>
#include <QGraphicsScene>
#include <memory>
#include <QPointF>
#include <unordered_set>
#include "routetypes.h"

class Graph
{
public:
    static const inline std::vector<int> ALLOWED_RANDOM_WEIGHTS = {1,2,4,5,6,7,8,10,15,21};


    Graph(QGraphicsScene* scene);
    Node* add_node(std::unique_ptr<Node> node);
    std::vector<Node*> getAllNodes() const;
    void connect(Node* nodeA, Node* nodeB, const ChannelProperties& props);
    double computeTransmissionTime(const ChannelProperties& ch, int packetBytes);
    void removeConnection(Node* a, Node* b);
    void removeNode(Node* node);
    void applyForceDirectedLayout();
    double edgeCost(const ChannelProperties& p, RouteMetric metric) const;

    RouteTable computeRoutingFrom(Node* src, RouteMetric metric);
    std::vector<Node*> shortestPath(Node* src, Node* dst, RouteMetric metric);


private:

    QGraphicsScene* scene;
    std::vector<std::unique_ptr<Node>> nodes;
    std::unordered_set<Node*> nodes_search;

};

#endif // GRAPH_H
