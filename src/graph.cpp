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
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QDebug>


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
    double transmission = (double)packetBytes * edgeCost(ch, RouteMetric::EffectiveCost) * 0.001;

    double propagation = ch.mode == ChannelMode::Satellite ?
                            SATELLITE_LATENCY:
                            0.01;

    double t = transmission + propagation;

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
    if (nodes.empty()) return;

    auto doFallbackRadial = [this]() {
        const double CX = 500.0, CY = 350.0, R = 300.0;
        const int n = static_cast<int>(nodes.size());
        for (int i = 0; i < n; ++i) {
            const double ang = (2.0 * M_PI * i) / n;
            nodes[i]->setPos(CX + R * std::cos(ang), CY + R * std::sin(ang));
        }
        qWarning() << "[layout] Fallback radial applied.";
    };

    const QString appDir = QCoreApplication::applicationDirPath();

#ifdef Q_OS_WIN
    const QString python = QDir(appDir).filePath("../../src/venv/Scripts/python.exe");
#else
    const QString python = QDir(appDir).filePath("../../src/venv/bin/python3");
#endif
    const QString script = QDir(appDir).filePath("../../src/layout.py");

    qDebug() << "[layout] checking paths:" << python << script;
    qDebug() << "[layout] appDir =" << appDir;
    qDebug() << "[layout] absolute python path =" << QDir(python).absolutePath();
    qDebug() << "[layout] absolute script path =" << QDir(script).absolutePath();

    if (!QFile::exists(python) || !QFile::exists(script)) {
        qWarning() << "[layout] python or layout.py missing:" << python << script;
        doFallbackRadial();
        return;
    }

    QJsonObject root;
    root.insert("node_count", static_cast<int>(nodes.size()));

    QJsonArray edgesArr;
    std::unordered_map<Node*, int> index;
    index.reserve(nodes.size());
    for (int i = 0; i < static_cast<int>(nodes.size()); ++i)
        index[nodes[i].get()] = i;

    for (int i = 0; i < static_cast<int>(nodes.size()); ++i) {
        Node* a = nodes[i].get();
        for (auto &[b, props] : a->get_adj()) {
            if (!b) continue;
            auto it = index.find(b);
            if (it == index.end()) continue;
            int j = it->second;
            if (i < j) {
                QJsonArray e;
                e.append(i);
                e.append(j);
                e.append(props.weight);
                edgesArr.append(e);
            }
        }
    }

    root.insert("edges", edgesArr);
    QByteArray inputJson = QJsonDocument(root).toJson(QJsonDocument::Compact);

    if (edgesArr.isEmpty()) {
        qWarning() << "[layout] No edges to layout.";
        doFallbackRadial();
        return;
    }

    QProcess proc;
    proc.setProgram(python);
    proc.setArguments({script});
    proc.setWorkingDirectory(QFileInfo(script).absolutePath());

    qDebug() << "[layout] launching python:" << python;
    qDebug() << "[layout] layout.py:" << script;

    proc.start();
    if (!proc.waitForStarted(3000)) {
        qWarning() << "[layout] Failed to start Python:" << proc.errorString();
        doFallbackRadial();
        return;
    }

    proc.write(inputJson);
    proc.closeWriteChannel();

    if (!proc.waitForFinished(10000)) {
        qWarning() << "[layout] Python timeout.";
        proc.kill();
        doFallbackRadial();
        return;
    }

    QByteArray out = proc.readAllStandardOutput();
    QByteArray err = proc.readAllStandardError();

    if (!err.isEmpty())
        qWarning() << "[layout][stderr]" << err;

    QJsonParseError perr{};
    QJsonDocument parsed = QJsonDocument::fromJson(out, &perr);

    if (perr.error != QJsonParseError::NoError || !parsed.isObject()) {
        qWarning() << "[layout] Invalid JSON output from layout.py:" << perr.errorString();
        doFallbackRadial();
        return;
    }

    QJsonObject pos = parsed.object();
    for (int i = 0; i < static_cast<int>(nodes.size()); ++i) {
        const QJsonArray arr = pos[QString::number(i)].toArray();
        if (arr.size() < 2) continue;
        const double x = arr[0].toDouble() * 600 + 400;
        const double y = arr[1].toDouble() * 600 + 300;
        nodes[i]->setPos(x, y);
    }

    qDebug() << "[layout] Layout applied successfully.";
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
    if (metric == RouteMetric::MinHops){
        return 1.0;
    }
    double base_cost = 1.0;

    int ref_bw = 1000;
    base_cost = ref_bw / p.weight;
    if (metric == RouteMetric::VirtualCost && p.type == ChannelType::HalfDuplex) {
        base_cost *= 2;
    }
    return base_cost + (p.mode == ChannelMode::Satellite ? 250 : 0);

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

            if (!props.active)
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

void Graph::randomizeChannelTypes()
{
    forEachActiveLink([&](Node* a, Node* b, const ChannelProperties& oldProps) {
        ChannelType t = (rand() % 2 == 0)
        ? ChannelType::Duplex
        : ChannelType::HalfDuplex;

        a->update_adj_type(b, t);
        b->update_adj_type(a, t);
    });

}

void Graph::setAllChannelsType(ChannelType type)
{
    forEachActiveLink([&](Node* a, Node* b, const ChannelProperties& oldProps) {
        ChannelProperties newProps = oldProps;
        newProps.type = type;

        a->update_adj_type(b, type);
        b->update_adj_type(a, type);

        for (auto item : scene->items()) {
            if (auto e = dynamic_cast<EdgeItem*>(item)) {
                if ((e->a == a && e->b == b) || (e->a == b && e->b == a)) {
                    e->updateStyle(newProps);
                }
            }
        }
    });

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




