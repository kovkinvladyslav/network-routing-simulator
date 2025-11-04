#include "edgeitem.h"
#include "node.h"
#include <QPen>

EdgeItem::EdgeItem(Node* a, Node* b, int weight)
    : a(a), b(b)
{
    setPen(QPen(Qt::white, 2));
    updatePosition();
}

void EdgeItem::updatePosition()
{
    QPointF a_center = a->pos() + a->boundingRect().center();
    QPointF b_center = b->pos() + b->boundingRect().center();
    setLine(QLineF(a_center, b_center));
}
