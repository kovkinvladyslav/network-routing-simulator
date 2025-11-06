#include "graph.h"

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

void Graph::connect(Node* nodeA, Node* nodeB, ChannelType type, ChannelMode mode, int weight)
{
    nodeA->add_adj(nodeB, weight, type, mode);
    nodeB->add_adj(nodeA, weight, type, mode);
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
    if (nodes.size() <= 1) return;

    const double width = 800;
    const double height = 600;
    const double area = width * height;
    const double k = sqrt(area / nodes.size());
    const int iterations = 200;
    const double initialTemp = 40.0;

    std::unordered_map<Node*, QPointF> disp;

    for (int it = 0; it < iterations; ++it) {
        double temperature = initialTemp * (1.0 - double(it) / iterations);

        for (auto &n : nodes)
            disp[n.get()] = QPointF(0, 0);

        // repulsion
        for (auto &a : nodes)
            for (auto &b : nodes)
                if (a.get() != b.get()) {
                    QPointF delta = a->pos() - b->pos();
                    double dist = std::hypot(delta.x(), delta.y());
                    if (dist < 0.01) dist = 0.01;
                    double force = (k * k) / dist;
                    disp[a.get()] += QPointF(delta.x() / dist * force,
                                             delta.y() / dist * force);
                }

        // attraction
        for (auto &a : nodes) {
            for (auto &[b, w] : a->get_adj()) {
                QPointF delta = a->pos() - b->pos();
                double dist = std::hypot(delta.x(), delta.y());
                if (dist < 0.01) dist = 0.01;
                double force = (dist * dist) / k;
                disp[a.get()] -= QPointF(delta.x() / dist * force,
                                         delta.y() / dist * force);
            }
        }

        for (auto &a : nodes) {
            QPointF d = disp[a.get()];
            double mag = std::hypot(d.x(), d.y());
            if (mag > temperature) {
                d *= temperature / mag;
            }

            QPointF newPos = a->pos() + d;
            newPos.setX(std::clamp(newPos.x(), 50.0, width - 50.0));
            newPos.setY(std::clamp(newPos.y(), 50.0, height - 50.0));
            a->setPos(newPos);
        }
    }
}
