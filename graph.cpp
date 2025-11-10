#include "graph.h"
#include <QProcess>
#include <QRegularExpression>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QMessageBox>
#include <QCoreApplication>
#include "djkstrastepper.h"


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

void Graph::connect(Node* nodeA, Node* nodeB, const ChannelProperties& props)
{
    nodeA->add_adj(nodeB, props);
    nodeB->add_adj(nodeA, props);
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

void Graph::applyForceDirectedLayout()
{
    if (nodes.size() < 2) return;

    QString base = QCoreApplication::applicationDirPath() + "/../..";
    QString python = base + "/venv/Scripts/python.exe";
    QString script = base + "/layout.py";

    if (!QFile::exists(python) || !QFile::exists(script))
        return;

    QJsonObject root;
    root["node_count"] = int(nodes.size());

    std::unordered_map<Node*, int> index;
    for (int i = 0; i < (int)nodes.size(); ++i)
        index[nodes[i].get()] = i;

    QJsonArray edgesArr;
    for (int i = 0; i < (int)nodes.size(); ++i)
        for (auto &[nbr, props] : nodes[i]->get_adj()) {
            int j = index[nbr];
            if (i < j) {
                QJsonArray e;
                e.append(i);
                e.append(j);
                e.append(props.weight);
                edgesArr.append(e);
            }
        }

    root["edges"] = edgesArr;
    QByteArray inputJson = QJsonDocument(root).toJson(QJsonDocument::Compact);

    QProcess *p = new QProcess();
    p->setProgram(python);
    p->setArguments({ script });

    QObject::connect(p, &QProcess::started, [p, inputJson]() {
        p->write(inputJson);
        p->closeWriteChannel();
    });

    QObject::connect(p, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                     [this, p](int, QProcess::ExitStatus) {

                         QByteArray out = p->readAllStandardOutput();
                         QByteArray err = p->readAllStandardError();

                         if (!err.isEmpty())
                             qDebug() << "[PYTHON STDERR]" << err;

                         QJsonParseError parseErr{};
                         QJsonDocument parsed = QJsonDocument::fromJson(out, &parseErr);
                         if (parseErr.error != QJsonParseError::NoError) {
                             qDebug() << "JSON parse error:" << parseErr.errorString();
                             p->deleteLater();
                             return;
                         }

                         QJsonObject pos = parsed.object();

                         for (int i = 0; i < (int)nodes.size(); ++i) {
                             QJsonArray arr = pos[QString::number(i)].toArray();
                             if (arr.size() < 2) continue;
                             double x = arr[0].toDouble() * 600 + 400;
                             double y = arr[1].toDouble() * 600 + 300;
                             nodes[i]->setPos(x, y);
                         }

                         qDebug() << "Layout applied successfully (finished event).";
                         p->deleteLater();
                     });

    p->start();
}


void Graph::removeNode(Node* node)
{
    if (!node) return;

    std::vector<Node*> nbrs;
    nbrs.reserve(node->get_adj().size());
    for (auto& [nbr, _] : node->get_adj()) nbrs.push_back(nbr);
    for (Node* nbr : nbrs) {
        nbr->removeAdj(node);
    }

    scene->removeItem(node);
    nodes_search.erase(node);

    auto it = std::find_if(nodes.begin(), nodes.end(),
                           [&](const std::unique_ptr<Node>& p){ return p.get() == node; });
    if (it != nodes.end()) nodes.erase(it);
}


double Graph::edgeCost(const ChannelProperties& p, RouteMetric metric) const {
    if (metric == RouteMetric::MinHops)
        return 1.0;

    double type_factor = (p.type == ChannelType::Duplex ? 1.0 : 1.2);
    double mode_factor = (p.mode == ChannelMode::Satellite ? 1.5 : 1.0);
    double retry_factor = 1.0 / (1.0 - p.errorProb);

    return p.weight * type_factor * mode_factor * retry_factor;
}

RouteTable Graph::computeRoutingFrom(Node* src, RouteMetric metric)
{
    RouteTable table;
    std::unordered_map<Node*, double> dist;
    std::unordered_map<Node*, Node*> parent;

    for (auto& np : nodes) {
        Node* v = np.get();
        dist[v] = 1e18;
        table[v] = RouteEntry{};
    }

    dist[src] = 0;
    table[src].cost = 0;
    table[src].hops = 0;

    std::vector<Node*> q;
    for (auto& np : nodes) q.push_back(np.get());

    while (!q.empty()) {
        Node* u = nullptr;
        double best = 1e18;
        for (Node* v : q) {
            if (dist[v] < best) {
                best = dist[v];
                u = v;
            }
        }
        if (!u) break;

        q.erase(std::remove(q.begin(), q.end(), u), q.end());

        for (auto& [nbr, props] : u->get_adj()) {
            if (u->getState() != NodeState::ON || nbr->getState() != NodeState::ON)
                continue;

            double cost = edgeCost(props, metric);
            if (dist[u] + cost < dist[nbr]) {
                dist[nbr] = dist[u] + cost;
                parent[nbr] = u;
            }
        }
    }

    for (auto& np : nodes) {
        Node* v = np.get();
        if (v == src) continue;

        Node* cur = v;
        Node* prev = nullptr;
        while (parent.count(cur) && parent[cur] != src)
            cur = parent[cur];
        if (parent.count(v)) prev = (parent[v] == src ? v : cur);

        if (parent.count(v)) {
            table[v].nextHop = prev;
            table[v].cost = dist[v];

            int hops = 0;
            Node* t = v;
            while (parent.count(t)) {
                t = parent[t];
                hops++;
            }
            table[v].hops = hops;
        }
    }

    return table;
}

std::vector<Node*> Graph::shortestPath(Node* src, Node* dst, RouteMetric metric)
{
    auto table = computeRoutingFrom(src, metric);
    std::vector<Node*> path;

    if (src == dst) return { src };

    Node* cur = dst;
    while (cur && cur != src) {
        path.push_back(cur);
        cur = table[cur].nextHop;
    }
    if (!cur) return {};
    path.push_back(src);
    std::reverse(path.begin(), path.end());
    return path;
}

void Graph::invalidateLSR()
{
    lsrComplete = false;
    lsrStarted = false;
    forEachActiveLink([&](Node *r, Node *, const ChannelProperties&) {
        r->topologyTable.clear();
    });
}


bool Graph::performLSAStep() {
    bool changed = false;
    std::unordered_map<Node*, LSATable> tempDB;

    for(auto &np : nodes) {
        tempDB[np.get()] = np->topologyTable;
    }

    forEachActiveLink([&](Node *r, Node *nbr, const ChannelProperties&) {
        for(auto &entry : r->topologyTable) {
            changed |= tempDB[nbr].insert(entry).second;
        }
    });

    if(!changed) {
        lsrComplete = true;
        return false;
    }

    for(auto &np : nodes) {
        Node *r = np.get();
        if(r->getState() == NodeState::ON) {
            r->topologyTable = std::move(tempDB[r]);
        }
    }
    return true;
}

void Graph::LSRInit()
{
    lsrStarted = true;
    lsrComplete = false;

    forEachActiveLink([&](Node *r, Node *nbr, const ChannelProperties &props) {
        r->topologyTable.insert(TopologyEntry(r, nbr, props));
    });
}

std::vector<Node *> Graph::getShortestPath(Node *src, Node *dst, RouteMetric metric)
{
    DijkstraStepper stepper(this, src, metric);
    DijkstraStep tmp;

    while(stepper.step(tmp)){}
    return stepper.getPathTo(dst);
}

void Graph::forEachActiveLink(std::function<void (Node *, Node *, const ChannelProperties &)> fn)
{
    for(auto &np : nodes) {
        Node *r = np.get();
        if(r->getState() != NodeState::ON) {
            continue;
        }
        for(auto &[nbr, props] : r->get_adj()) {
            if(nbr->getState() != NodeState::ON) {
                continue;
            }
            fn(r, nbr, props);
        }
    }
}




