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

    void addEdge(Node* a, Node* b, int weight);

private slots:
    void updateEdges();

private:
    QGraphicsScene* scene;
    std::vector<std::unique_ptr<EdgeItem>> edges;
};

#endif // EDGECONTROLLER_H
