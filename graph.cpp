#include "graph.h"
#include <QProcess>
#include <QRegularExpression>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QMessageBox>
#include <QCoreApplication>


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

