#include "topologyentry.h"
#include "node.h"

TopologyEntry::TopologyEntry(Node *f, Node *t, ChannelProperties p)
    : from(f), to(t), props(p){}

bool TopologyEntry::operator<(const TopologyEntry &other) const {
    if(from->getId() != other.from->getId()) {
        return from->getId() < other.from->getId();
    }
    return to->getId() < other.to->getId();
}
