#ifndef EDGECONTROLLER_H
#define EDGECONTROLLER_H

#include <QObject>
#include <vector>
#include <memory>
#include "edgeitem.h"
#include "graph.h"

class QGraphicsScene;
class Node;

class EdgeController : public QObject
{
    Q_OBJECT
public:
    EdgeController(QGraphicsScene* scene, Graph* graph);
    void addEdge(Node* a, Node* b, const ChannelProperties& props);
    void removeEdge(EdgeItem* edge);
    EdgeItem* findEdge(Node* a, Node* b);
    void watchNode(Node* node);
    void highlightPath(const std::vector<Node*>& path);
    void highlightRelaxations(const std::vector<std::pair<Node*,Node*>>& relaxEdges);
    void clearHighlight();

signals:
    void routingChanged();

private slots:
    void updateEdges();

private:
    Graph* graph;
    QGraphicsScene* scene;
    std::vector<std::unique_ptr<EdgeItem>> edges;
};

#endif // EDGECONTROLLER_H
