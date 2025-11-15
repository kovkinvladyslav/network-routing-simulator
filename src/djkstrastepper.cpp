#include "djkstrastepper.h"
#include "graph.h"
#include "node.h"
#include <algorithm>

DijkstraStepper::DijkstraStepper(Graph* g, Node* src, RouteMetric metric)
    : graph(g), source(src), metric(metric)
{
    for (auto& np : g->getAllNodes()) {
        Node* node = np;
        dist[node] = 1e18;
        queue.push_back(node);
    }
    dist[src] = 0;
    parent[src] = nullptr;
}

bool DijkstraStepper::finished() const {
    return done;
}

std::vector<Node*> DijkstraStepper::getPathTo(Node* dst) {
    std::vector<Node*> path;
    if (!parent.count(dst) && dst != source) return path;

    Node* cur = dst;
    while (cur) {
        path.push_back(cur);
        if (cur == source) break;
        cur = parent[cur];
    }
    std::reverse(path.begin(), path.end());
    return path;
}

bool DijkstraStepper::step(DijkstraStep& out)
{
    if (done) return false;

    out.relaxedEdges.clear();
    out.unvisited.clear();
    out.current.clear();
    out.settled = nullptr;
    out.nextToSettle = nullptr;


    Node* u = nullptr;
    double best = 1e18;
    for (Node* v : queue) {
        if (dist[v] < best) {
            best = dist[v];
            u = v;
        }
    }

    if (!u || best >= 1e18) {
        done = true;
        return false;
    }

    queue.erase(std::remove(queue.begin(), queue.end(), u), queue.end());
    out.settled = u;

    for (auto& [nbr, props] : u->get_adj()) {
        if (nbr->getState() != NodeState::ON || u->getState() != NodeState::ON)
            continue;
        if (!props.active)
            continue;

        if (std::find(queue.begin(), queue.end(), nbr) == queue.end())
            continue;

        double cost = graph->edgeCost(props, metric);
        double newDist = dist[u] + cost;

        if (newDist < dist[nbr]) {
            dist[nbr] = newDist;
            parent[nbr] = u;
            out.relaxedEdges.push_back({u, nbr});
        }
    }

    for (auto& [node, d] : dist) {
        RouteEntry entry;
        entry.cost = d;

        if (node == source) {
            entry.nextHop = nullptr;
            entry.hops = 0;
        } else if (parent.count(node)) {
            Node* cur = node;
            while (parent.count(cur) && parent[cur] != source && parent[cur] != nullptr) {
                cur = parent[cur];
            }
            entry.nextHop = (parent.count(node) && parent[node] == source) ? node : cur;

            int hops = 0;
            Node* t = node;
            while (parent.count(t) && parent[t] != nullptr) {
                t = parent[t];
                hops++;
            }
            entry.hops = hops;
        }

        out.current[node] = entry;
    }

    if (!queue.empty()) {
        double nextBest = 1e18;
        for (Node* v : queue) {
            if (dist[v] < nextBest) {
                nextBest = dist[v];
                out.nextToSettle = v;
            }
        }
    }

    out.unvisited = queue;

    if (queue.empty()) {
        done = true;
    }

    return true;
}
