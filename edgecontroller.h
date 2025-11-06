#ifndef EDGECONTROLLER_H
#define EDGECONTROLLER_H

#include <QObject>
#include <vector>
#include <memory>
#include "edgeitem.h"

class QGraphicsScene;
class Node;
class EdgeItem;

class EdgeController : public QObject
{
    Q_OBJECT
public:
    EdgeController(QGraphicsScene* scene);
    void addEdge(Node* a, Node* b, const ChannelProperties& props);
    void removeEdge(EdgeItem* edge);
    EdgeItem* findEdge(Node* a, Node* b);
    void watchNode(Node* node);

signals:
    void routingChanged();

private slots:
    void updateEdges();

private:
    QGraphicsScene* scene;
    std::vector<std::unique_ptr<EdgeItem>> edges;
};

#endif // EDGECONTROLLER_H
