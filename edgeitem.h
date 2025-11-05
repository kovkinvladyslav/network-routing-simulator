#ifndef EDGEITEM_H
#define EDGEITEM_H

#include <QGraphicsLineItem>

class Node;
enum class ChannelType { Duplex, HalfDuplex };
enum class ChannelMode { Normal, Satellite };

class EdgeItem : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    EdgeItem(Node* a, Node* b, int weight, ChannelType type, ChannelMode mode);

    Node* a;
    Node* b;
    int weight;
    ChannelType type;
    ChannelMode mode;

    void updatePosition();
signals:
    void requestRemove(EdgeItem* edge);

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

};

#endif // EDGEITEM_H
