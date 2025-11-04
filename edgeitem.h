#ifndef EDGEITEM_H
#define EDGEITEM_H

#include <QGraphicsLineItem>

class Node;

class EdgeItem : public QGraphicsLineItem
{
public:
    EdgeItem(Node* a, Node* b, int weight);

    void updatePosition();

private:
    Node* a;
    Node* b;
};

#endif // EDGEITEM_H
