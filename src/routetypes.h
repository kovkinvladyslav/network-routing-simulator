#ifndef ROUTETYPES_H
#define ROUTETYPES_H
#include <unordered_map>

class Node;

enum class RouteMetric {
    EffectiveCost,
    MinHops
};

struct RouteEntry {
    Node* nextHop = nullptr;
    double cost = 1e18;
    int hops = 1e9;
};

using RouteTable = std::unordered_map<Node*, RouteEntry>;

#endif // ROUTETYPES_H
