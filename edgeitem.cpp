#include "edgeitem.h"
#include "node.h"
#include <QPen>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>

EdgeItem::EdgeItem(Node* a, Node* b, const ChannelProperties& props)
    : QObject(), QGraphicsLineItem(), a(a), b(b), props(props)
{
    QPen pen(Qt::white, 2);

    if (mode == ChannelMode::Satellite)
        pen.setStyle(Qt::DashLine);
    else
        pen.setStyle(Qt::SolidLine);

    setToolTip(QString("Weight: %1\nType: %2\nMode: %3\nError prob: %4")
                   .arg(props.weight)
                   .arg(props.type == ChannelType::Duplex ? "Duplex" : "Half-Duplex")
                   .arg(props.mode == ChannelMode::Satellite ? "Satellite" : "Normal")
                   .arg(props.errorProb, 0, 'f', 3));



    setPen(pen);
    setZValue(-1);
    if (!active) {
        QPen p = this->pen();
        p.setColor(Qt::gray);
        p.setStyle(Qt::DashDotLine);
        setPen(p);
    }

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
    QAction* toggle = menu.addAction(active ? "Disable Channel" : "Enable Channel");
    QAction* remove = menu.addAction("Remove Channel");

    QAction* chosen = menu.exec(event->screenPos());
    if (!chosen) return;

    if (chosen == toggle) {
        active = !active;

        QPen p = pen();
        if (active) {
            p.setColor(Qt::white);
            p.setStyle(mode == ChannelMode::Satellite ? Qt::DashLine : Qt::SolidLine);
        } else {
            p.setColor(Qt::gray);
            p.setStyle(Qt::DashDotLine);
        }
        setPen(p);

        setToolTip(QString("Weight: %1\nType: %2\nMode: %3\nState: %4")
                       .arg(weight)
                       .arg(type == ChannelType::Duplex ? "Duplex" : "Half-Duplex")
                       .arg(mode == ChannelMode::Satellite ? "Satellite" : "Normal")
                       .arg(active ? "Active" : "Disabled"));

        emit channelStateChanged();
    }

    if (chosen == remove) {
        emit requestRemove(this);
    }
}
