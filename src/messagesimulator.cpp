#include "messagesimulator.h"
#include "graph.h"
#include <cmath>

#include "messagesimulator.h"
#include "graph.h"
#include <cmath>

MessageSimulationResult MessageSimulator::sendDatagram(
    Graph* graph, Node* src, Node* dst,
    int messageSizeBytes, int packetSizeBytes)
{
    MessageSimulationResult result{};
    result.totalPackets = std::ceil((double)messageSizeBytes / packetSizeBytes);
    int trail = messageSizeBytes % packetSizeBytes;
    messageSizeBytes = messageSizeBytes/packetSizeBytes;
    auto path = graph->getShortestPath(src, dst, RouteMetric::EffectiveCost);
    if (path.size() < 2) {
        result.delivered = false;
        return result;
    }

    for (int i = 0; i < result.totalPackets; ++i) {
        result.totalTime += sendBytes(graph, src, dst, messageSizeBytes + datagram_header_size);
        result.serviceDataSize += datagram_header_size;
    }
    if(trail){
        result.totalTime += sendBytes(graph, src, dst, trail + datagram_header_size);
        result.serviceDataSize += datagram_header_size;
    }
    result.delivered = true;
    result.lastPath = path;
    return result;
}




MessageSimulationResult MessageSimulator::sendVirtualCircuit(
    Graph* graph, Node* src, Node* dst,
    int messageSizeBytes, int packetSizeBytes)
{
    MessageSimulationResult result{};
    result.totalPackets = std::ceil((double)messageSizeBytes / packetSizeBytes) + service_packets;

    auto path = graph->getShortestPath(src, dst, RouteMetric::EffectiveCost);
    if (path.size() < 2) {
        result.delivered = false;
        return result;
    }

    int trail = messageSizeBytes % packetSizeBytes;
    messageSizeBytes = messageSizeBytes / packetSizeBytes;
    result.serviceDataSize += virtual_header_size * 7;

    for(int i = 0; i < result.totalPackets; i++) {
        result.totalTime += sendBytes(graph, src, dst, packetSizeBytes + virtual_header_size);
        result.totalTime += sendBytes(graph, dst, src, virtual_header_size);
        result.serviceDataSize += virtual_header_size;
    }
    if(trail){
        result.totalTime += sendBytes(graph, src, dst, trail + virtual_header_size);
        result.serviceDataSize += virtual_header_size;
    }

    result.delivered = true;
    result.lastPath = path;
    return result;
}

double MessageSimulator::sendBytes(Graph *graph, Node *from, Node *to, int bytes)
{
    auto path = graph->getShortestPath(from, to, RouteMetric::EffectiveCost);
    if (path.size() < 2)
        return 0.0;

    double totalTime = 0.0;
    for (size_t i = 0; i + 1 < path.size(); ++i) {
        Node* a = path[i];
        Node* b = path[i + 1];
        const auto& props = a->get_adj().at(b);
        totalTime += graph->computeTransmissionTime(props, bytes);
    }
    return totalTime;
}
