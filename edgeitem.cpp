#include "edgeitem.h"
#include "node.h"
#include <QPen>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>

EdgeItem::EdgeItem(Node* a, Node* b, int weight, ChannelType type, ChannelMode mode)
    : QObject(), QGraphicsLineItem(), a(a), b(b), weight(weight), type(type), mode(mode)
{
    QPen pen(Qt::white, 2);

    if (mode == ChannelMode::Satellite)
        pen.setStyle(Qt::DashLine);
    else
        pen.setStyle(Qt::SolidLine);

    setToolTip(QString("Weight: %1\nType: %2\nMode: %3")
                   .arg(weight)
                   .arg(type == ChannelType::Duplex ? "Duplex" : "Half-Duplex")
                   .arg(mode == ChannelMode::Satellite ? "Satellite" : "Normal"));


    setPen(pen);
    setZValue(-1);
    updatePosition();
}

void EdgeItem::updatePosition()
{
    QPointF ac = a->pos() + a->boundingRect().center();
    QPointF bc = b->pos() + b->boundingRect().center();
    setLine(QLineF(ac, bc));
}

void EdgeItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    QAction* remove = menu.addAction("Видалити канал");
    QAction* chosen = menu.exec(event->screenPos());
    if (!chosen) return;

    if (chosen == remove) {
        emit requestRemove(this);
    }
}

