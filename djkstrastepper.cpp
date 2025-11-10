#include "djkstrastepper.h"
#include "graph.h"
#include "node.h"

DijkstraStepper::DijkstraStepper(Graph* g, Node* src, RouteMetric metric)
    : graph(g), source(src), metric(metric)
{
    for (auto& np : g->getAllNodes()) {
        dist[np] = 1e18;
        queue.push_back(np);
    }
    dist[src] = 0;
}

bool DijkstraStepper::finished() const { return done; }

std::vector<Node*> DijkstraStepper::getPathTo(Node* dst) {
    std::vector<Node*> path;
    if (!parent.count(dst) && dst != source) return path;

    Node* cur = dst;
    while (cur != source) {
        path.push_back(cur);
        cur = parent.at(cur);
    }
    path.push_back(source);
    std::reverse(path.begin(), path.end());
    return path;
}

bool DijkstraStepper::step(DijkstraStep& out)
{
    if (done) return false;

    Node* u = nullptr;
    double best = 1e18;
    for (Node* v : queue) {
        if (dist[v] < best) { best = dist[v]; u = v; }
    }
    if (!u) { done = true; return false; }

    queue.erase(std::remove(queue.begin(), queue.end(), u), queue.end());
    out.settled = u;

    for (auto& [nbr, props] : u->get_adj()) {
        if (nbr->getState() != NodeState::ON)
            continue;
        if (u->getState() != NodeState::ON)
            continue;

        double cost = graph->edgeCost(props, metric);
        if (dist[u] + cost < dist[nbr]) {
            dist[nbr] = dist[u] + cost;
            parent[nbr] = u;
            out.relaxedEdges.push_back({u, nbr});
        }
    }

    for (auto& [v, d] : dist) {
        RouteEntry r;
        r.cost = d;

        Node* cur = v;
        Node* prev = nullptr;
        while (parent.count(cur) && parent[cur] != source)
            cur = parent[cur];
        if (parent.count(v)) prev = (parent[v] == source ? v : cur);

        r.nextHop = prev;

        int hops = 0; Node* t = v;
        while (parent.count(t)) { t = parent[t]; hops++; }
        r.hops = hops;

        out.current[v] = r;
    }
    return true;
}
