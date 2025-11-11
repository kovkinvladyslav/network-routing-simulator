#ifndef TOPOLOGYENTRY_H
#define TOPOLOGYENTRY_H

#include "ChannelProperties.h"

class Node;

class TopologyEntry
{
public:
    Node *to;
    Node *from;
    ChannelProperties props;

    TopologyEntry(Node *f, Node *t, ChannelProperties p);

    bool operator<(const TopologyEntry &other) const;
};

#endif // TOPOLOGYENTRY_H
