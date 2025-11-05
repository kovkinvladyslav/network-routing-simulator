#include "node.h"
#include <QBrush>
#include <QPoint>
#include <QPen>

Node::Node(int id, QPointF pos)
    : QGraphicsEllipseItem(-20, -20, 20, 20), id(id)
{
    setState(NodeState::DISCONNECTED);
    setPos(pos);
    setFlags(ItemIsMovable | ItemIsSelectable | ItemSendsGeometryChanges);

    label = new QGraphicsTextItem(QString::number(id), this);
    label->setDefaultTextColor(Qt::white);

    QRectF bounds = this->boundingRect();
    label->setPos(bounds.center().x() - label->boundingRect().width()/2,
                  bounds.center().y() - label->boundingRect().height()/2);
}

const std::unordered_map<Node*, ChannelProperties> &Node::get_adj() const
{
    return adj_nodes;
}

int Node::getId()
{
    return id;
}

void Node::add_adj(Node *other, int weight, ChannelType type, ChannelMode mode)
{
    setState(NodeState::ON);
    other->setState(NodeState::ON);
    adj_nodes[other] = { weight, type, mode };
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

void Node::highlight(bool on)
{
    if(on){
        setPen(QPen(Qt::yellow, 3));
    } else {
        setPen(Qt::NoPen);
    }
}

void Node::removeAdj(Node *other)
{
    adj_nodes.erase(other);

    if (adj_nodes.empty()) {
        setState(NodeState::DISCONNECTED);
    }
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked(this);
    QGraphicsEllipseItem::mousePressEvent(event);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionHasChanged) {
        emit moved();
    }

    if (change == QGraphicsItem::ItemSelectedHasChanged) {
        bool selected = value.toBool();
        highlight(selected);
    }

    return QGraphicsEllipseItem::itemChange(change, value);
}

