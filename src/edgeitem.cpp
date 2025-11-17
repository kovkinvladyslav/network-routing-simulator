#include "edgeitem.h"
#include "node.h"
#include <QPen>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>

EdgeItem::EdgeItem(Node* a, Node* b, const ChannelProperties& props)
    : QObject(), QGraphicsLineItem(), a(a), b(b), props(props)
{
    QPen pen(Qt::white, 2);

    ChannelProperties freshProps = a->get_adj().at(b);

    QPen p = this->pen();
    if (active) {
        p.setColor(Qt::white);
        p.setStyle(freshProps.mode == ChannelMode::Satellite ? Qt::DashLine : Qt::SolidLine);
    } else {
        p.setColor(Qt::gray);
        p.setStyle(Qt::DashDotLine);
    }
    setPen(p);
    setZValue(-1);

    setToolTip(QString("Weight: %1\nType: %2\nMode: %3\nState: %4\nErrProb: %5")
                   .arg(freshProps.weight)
                   .arg(freshProps.type == ChannelType::Duplex ? "Duplex" : "Half-Duplex")
                   .arg(freshProps.mode == ChannelMode::Satellite ? "Satellite" : "Normal")
                   .arg(active ? "Active" : "Disabled")
                   .arg(freshProps.errorProb, 0, 'f', 3));

    updatePosition();
}

void EdgeItem::updatePosition()
{
    QPointF ac = a->pos() + a->boundingRect().center();
    QPointF bc = b->pos() + b->boundingRect().center();
    setLine(QLineF(ac, bc));
}

void EdgeItem::updateStyle(const ChannelProperties &props)
{
    this->props = props;
    QPen p = pen();
    if (active) {
        p.setColor(Qt::white);
        p.setStyle(props.mode == ChannelMode::Satellite ? Qt::DashLine : Qt::SolidLine);
    } else {
        p.setColor(Qt::gray);
        p.setStyle(Qt::DashDotLine);
    }
    setPen(p);

    setToolTip(QString("Weight: %1\nType: %2\nMode: %3\nState: %4\nErrProb: %5")
                   .arg(props.weight)
                   .arg(props.type == ChannelType::Duplex ? "Duplex" : "Half-Duplex")
                   .arg(props.mode == ChannelMode::Satellite ? "Satellite" : "Normal")
                   .arg(active ? "Active" : "Disabled")
                   .arg(props.errorProb, 0, 'f', 3));
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

        props.active = active;
        a->updateChannelProperty(b, props);
        b->updateChannelProperty(a, props);

        QPen p = pen();
        if (active) {
            p.setColor(Qt::white);
            p.setStyle(props.mode == ChannelMode::Satellite ? Qt::DashLine : Qt::SolidLine);
        } else {
            p.setColor(Qt::gray);
            p.setStyle(Qt::DashDotLine);
        }
        setPen(p);

        setToolTip(QString("Weight: %1\nType: %2\nMode: %3\nState: %4\nErrProb: %5")
                       .arg(props.weight)
                       .arg(props.type == ChannelType::Duplex ? "Duplex" : "Half-Duplex")
                       .arg(props.mode == ChannelMode::Satellite ? "Satellite" : "Normal")
                       .arg(active ? "Active" : "Disabled")
                       .arg(props.errorProb, 0, 'f', 3));

        emit channelStateChanged();
    }

    if (chosen == remove) {
        emit requestRemove(this);
    }
}
