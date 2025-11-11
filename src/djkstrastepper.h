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
};

class DijkstraStepper {
public:
    DijkstraStepper(Graph* g, Node* src, RouteMetric metric);
    bool step(DijkstraStep& out);
    bool finished() const;
    std::vector<Node*> getPathTo(Node *dst);

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
