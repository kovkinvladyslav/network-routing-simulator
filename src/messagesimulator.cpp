#include "messagesimulator.h"
#include "graph.h"
#include <cmath>

MessageSimulationResult MessageSimulator::sendDatagram(
    Graph* graph, Node* src, Node* dst,
    int messageSizeBytes, int packetSizeBytes)
{
    MessageSimulationResult result{};

    auto path = graph->getShortestPath(src, dst, RouteMetric::DataGramCost);
    if (path.size() < 2) {
        result.delivered = false;
        return result;
    }

    int fullPackets = messageSizeBytes / packetSizeBytes;
    int trailBytes = messageSizeBytes % packetSizeBytes;

    result.totalPackets = fullPackets + (trailBytes > 0 ? 1 : 0);

    for (int i = 0; i < fullPackets; ++i) {
        result.totalTime += sendBytes(graph, src, dst,
                                      packetSizeBytes + datagram_header_size);
        result.serviceDataSize += datagram_header_size;
    }

    if (trailBytes > 0) {
        result.totalTime += sendBytes(graph, src, dst,
                                      trailBytes + datagram_header_size);
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

    auto path = graph->getShortestPath(src, dst, RouteMetric::EffectiveCost);
    if (path.size() < 2) {
        result.delivered = false;
        return result;
    }

    int fullPackets = messageSizeBytes / packetSizeBytes;
    int trailBytes = messageSizeBytes % packetSizeBytes;

    result.totalPackets = fullPackets + (trailBytes > 0 ? 1 : 0) + service_packets;

    result.totalTime += sendBytes(graph, src, dst, virtual_header_size);
    result.serviceDataSize += virtual_header_size;

    result.totalTime += sendAck(graph, dst, src, virtual_header_size);
    result.serviceDataSize += virtual_header_size;

    for (int i = 0; i < fullPackets; ++i) {
        result.totalTime += sendBytes(graph, src, dst,
                                      packetSizeBytes + virtual_header_size);
        result.serviceDataSize += virtual_header_size;

        result.totalTime += sendAck(graph, dst, src, virtual_header_size);
        result.serviceDataSize += virtual_header_size;
    }

    if (trailBytes > 0) {
        result.totalTime += sendBytes(graph, src, dst,
                                      trailBytes + virtual_header_size);
        result.serviceDataSize += virtual_header_size;

        result.totalTime += sendAck(graph, dst, src, virtual_header_size);
        result.serviceDataSize += virtual_header_size;
    }

    result.totalTime += sendBytes(graph, src, dst, virtual_header_size);
    result.serviceDataSize += virtual_header_size;

    result.totalTime += sendAck(graph, dst, src, virtual_header_size);
    result.serviceDataSize += virtual_header_size;

    result.delivered = true;
    result.lastPath = path;
    return result;
}

double MessageSimulator::sendAck(Graph *graph, Node *from, Node *to, int size)
{
    return sendBytes(graph, from, to, size, true);
}

double MessageSimulator::sendBytes(Graph *graph, Node *from, Node *to, int bytes, bool skipDuplex)
{
    auto path = graph->getShortestPath(from, to, RouteMetric::VirtualCost);
    if (path.size() < 2)
        return 0.0;

    double totalTime = 0.0;

    for (size_t i = 0; i + 1 < path.size(); ++i) {
        Node* a = path[i];
        Node* b = path[i + 1];
        const auto& props = a->get_adj().at(b);

        if (skipDuplex && props.type == ChannelType::Duplex) {
            continue;
        }
        totalTime += graph->computeTransmissionTime(props, bytes);
    }

    return totalTime;
}
