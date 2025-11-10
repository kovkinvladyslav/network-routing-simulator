#ifndef MESSAGE_SIMULATOR_H
#define MESSAGE_SIMULATOR_H

#include "graph.h"
#include "node.h"

struct MessageSimulationResult {
    bool delivered = false;
    double totalTime = 0.0;
    int totalPackets = 0;
    int retransmissions = 0;
    std::vector<Node*> lastPath;
};


class MessageSimulator {
public:
    static MessageSimulationResult sendDatagram(
        Graph* graph,
        Node* src,
        Node* dst,
        int messageSizeBytes,
        int packetSizeBytes
        );

    static MessageSimulationResult sendVirtualCircuit(
        Graph* graph,
        Node* src,
        Node* dst,
        int messageSizeBytes,
        int packetSizeBytes
        );
};

#endif // MESSAGE_SIMULATOR_H
