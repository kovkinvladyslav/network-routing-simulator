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
    static MessageSimulationResult sendMessageDatagram(
        Graph* graph,
        Node* src,
        Node* dst,
        int messageSizeBytes,
        int packetSizeBytes
        );

    static MessageSimulationResult sendMessageVirtual(
        Graph* graph,
        Node* src,
        Node* dst,
        int messageSizeBytes,
        int packetSizeBytes
        );
private:
    static double sendBytes(Graph *graph, std::vector<Node*> path, int bytes, bool skipDuplex = false);
    static constexpr int datagram_header_size = 8;
    static constexpr int virtual_header_size = 20;
    static constexpr int service_packets = 4;
    static double sendAck(Graph *graph, std::vector<Node*> path);
    static void sendDatagram(double &totalTime, int &serviceBytes,
                             Graph *graph, Node *from, Node *to, int size);
    static void sendVirtual(double &totaltime, int &serviceBytes, std::vector<Node*> path,
                            Graph *graph, int size = 0);
};

#endif // MESSAGE_SIMULATOR_H
