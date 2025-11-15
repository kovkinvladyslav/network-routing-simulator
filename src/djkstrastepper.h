#ifndef DJKSTRASTEPPER_H
#define DJKSTRASTEPPER_H

#include "RouteTypes.h"
#include <unordered_map>
#include <vector>

class Graph;
class Node;

struct DijkstraStep {
    Node* settled = nullptr;
    std::vector<std::pair<Node*,Node*>> relaxedEdges;
    RouteTable current;
    std::vector<Node*> unvisited;
    Node* nextToSettle = nullptr;
};

class DijkstraStepper {
public:
    DijkstraStepper(Graph* g, Node* src, RouteMetric metric);
    bool step(DijkstraStep& out);
    bool finished() const;
    std::vector<Node*> getPathTo(Node *dst);

    const std::unordered_map<Node*, double>& getDistances() const { return dist; }
    const std::unordered_map<Node*, Node*>& getParents() const { return parent; }
    const std::vector<Node*>& getQueue() const { return queue; }
    Node* getSource() const { return source; }

private:
    Graph* graph;
    Node* source;
    RouteMetric metric;

    std::unordered_map<Node*, double> dist;
    std::unordered_map<Node*, Node*> parent;
    std::vector<Node*> queue;
    bool done = false;
};

#endif // DJKSTRASTEPPER_H
