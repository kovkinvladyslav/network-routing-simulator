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
    int serviceDataSize = 0;
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

private:
    static double sendBytes(Graph *graph, Node *from, Node *to, int bytes);
    static constexpr int datagram_header_size = 8;
    static constexpr int virtual_header_size = 20;
    static constexpr int service_packets = 7;
};

#endif // MESSAGE_SIMULATOR_H
