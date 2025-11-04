#ifndef NODE_H
#define NODE_H
#include <QGraphicsEllipseItem>
#include <unordered_set>
#include <QPointF>

enum class NodeState {
    ON,
    OFF,
    DISCONNECTED
};

class Node : public QGraphicsEllipseItem
{
public:
    Node(int id, QPointF pos);
    const std::unordered_set<Node*>& get_adj() const;
    void add_adj(Node *other);
    void setState(NodeState newState);
private:
    int id;
    NodeState state = NodeState::DISCONNECTED;
    std::unordered_set<Node*> adj_nodes;
    QGraphicsTextItem* label;
};

#endif // NODE_H
