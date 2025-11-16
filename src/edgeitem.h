#ifndef EDGEITEM_H
#define EDGEITEM_H
#include "ChannelProperties.h"
#include <QGraphicsLineItem>
class Node;

class EdgeItem : public QObject, public QGraphicsLineItem
{
    Q_OBJECT
public:
    EdgeItem(Node* a, Node* b, const ChannelProperties& props);
    ChannelProperties props;

    Node* a;
    Node* b;
    bool active = true;


    void updatePosition();
    void updateStyle(const ChannelProperties& props);
signals:
    void requestRemove(EdgeItem* edge);
    void channelStateChanged();


protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

};

#endif // EDGEITEM_H
