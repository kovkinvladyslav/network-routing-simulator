#include "node.h"
#include <QBrush>
#include <QPoint>

Node::Node(int id, QPointF pos)
    : QGraphicsEllipseItem(-20, -20, 20, 20), id(id)
{
    setState(NodeState::DISCONNECTED);
    setPos(pos);
    setFlags(ItemIsMovable | ItemIsSelectable);

    label = new QGraphicsTextItem(QString::number(id), this);
    label->setDefaultTextColor(Qt::white);

    QRectF bounds = this->boundingRect();
    label->setPos(bounds.center().x() - label->boundingRect().width()/2,
                  bounds.center().y() - label->boundingRect().height()/2);
}

const std::unordered_set<Node*> &Node::get_adj() const
{
    return adj_nodes;
}

void Node::add_adj(Node *other)
{
    this->setState(NodeState::ON);
    other->setState(NodeState::ON);
    adj_nodes.insert(other);
}

void Node::setState(NodeState newState)
{
    state = newState;

    switch(state)
    {
    case NodeState::ON:
        setBrush(Qt::green);
        break;

    case NodeState::OFF:
        setBrush(Qt::red);
        break;

    case NodeState::DISCONNECTED:
        setBrush(Qt::gray);
        break;
    }
}

