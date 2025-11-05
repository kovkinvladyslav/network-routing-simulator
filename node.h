#ifndef NODE_H
#define NODE_H
#include <QGraphicsEllipseItem>
#include <unordered_map>
#include <QPointF>
#include <QObject>
#include "edgeitem.h"

enum class NodeState {
    ON,
    OFF,
    DISCONNECTED
};

struct ChannelProperties {
    int weight;
    ChannelType type;
    ChannelMode mode;
};


class Node : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    Node(int id, QPointF pos);
    const std::unordered_map<Node*, ChannelProperties>& get_adj() const;
    int getId();
    void add_adj(Node *other, int weight, ChannelType type, ChannelMode mode);
    void setState(NodeState newState);
    void highlight(bool on);
    void removeAdj(Node* other);
    const NodeState getState() const;
signals:
    void clicked(Node* self);
    void moved();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    int id;
    NodeState state = NodeState::DISCONNECTED;
    std::unordered_map<Node*, ChannelProperties> adj_nodes;
    QGraphicsTextItem* label;
};

#endif // NODE_H
