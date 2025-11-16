#include "messagesimulator.h"
#include "graph.h"
#include <cmath>
#include <QDebug>
#include <time.h>
#include <QDateTime>

MessageSimulationResult MessageSimulator::sendMessageDatagram(
    Graph* graph, Node* src, Node* dst,
    int messageSizeBytes, int packetSizeBytes)
{
    MessageSimulationResult result{};

    auto path = graph->getShortestPath(src, dst, RouteMetric::DataGramCost);

    int fullPackets = messageSizeBytes / packetSizeBytes;
    int trailBytes = messageSizeBytes % packetSizeBytes;

    result.totalPackets = fullPackets + (trailBytes > 0 ? 1 : 0);

    for (int i = 0; i < fullPackets; ++i) {
        sendDatagram(result.totalTime, result.serviceDataSize, graph, src, dst, packetSizeBytes);
    }

    if (trailBytes > 0) {
        sendDatagram(result.totalTime, result.serviceDataSize, graph, src, dst, trailBytes);
    }

    result.delivered = true;
    result.lastPath = path;
    return result;
}

MessageSimulationResult MessageSimulator::sendMessageVirtual(
    Graph* graph, Node* src, Node* dst,
    int messageSizeBytes, int packetSizeBytes)
{
    MessageSimulationResult result{};

    auto pathTo = graph->getShortestPath(src, dst, RouteMetric::VirtualCost);
    auto pathFrom = graph->getShortestPath(dst, src, RouteMetric::VirtualCost);
    if (pathTo.size() < 2) {
        result.delivered = false;
        return result;
    }

    int fullPackets = messageSizeBytes / packetSizeBytes;
    int trailBytes = messageSizeBytes % packetSizeBytes;

    result.totalPackets = 2 * (fullPackets + (trailBytes > 0 ? 1 : 0)) + service_packets;

    sendVirtual(result.totalTime, result.serviceDataSize, pathTo, graph);
    sendVirtual(result.totalTime, result.serviceDataSize, pathFrom, graph);

    for (int i = 0; i < fullPackets; ++i) {
        sendVirtual(result.totalTime, result.serviceDataSize, pathTo, graph, packetSizeBytes);
        result.totalTime += sendAck(graph, pathFrom);
    }

    if (trailBytes > 0) {
        sendVirtual(result.totalTime, result.serviceDataSize, pathTo, graph, trailBytes);
        result.totalTime += sendAck(graph, pathFrom);
    }

    sendVirtual(result.totalTime, result.serviceDataSize, pathTo, graph);
    sendVirtual(result.totalTime, result.serviceDataSize, pathFrom, graph);

    result.delivered = true;
    result.lastPath = pathTo;
    return result;
}

double MessageSimulator::sendAck(Graph *graph, std::vector<Node*> path)
{
    return sendBytes(graph, path, virtual_header_size, true);
}

void MessageSimulator::sendDatagram(double &totalTime, int &serviceBytes,
    Graph *graph, Node *from, Node *to, int size)
{
    auto path = graph->getShortestPath(from, to, RouteMetric::DataGramCost);

    if (path.size() < 2) return;
    totalTime += sendBytes(graph, path, size + datagram_header_size);
    serviceBytes += datagram_header_size;
}

void MessageSimulator::sendVirtual(double &totaltime, int &serviceBytes, std::vector<Node*> path,
                                   Graph *graph, int size)
{
    totaltime += sendBytes(graph, path, size + virtual_header_size);
    serviceBytes += virtual_header_size;
}

double MessageSimulator::sendBytes(Graph *graph, std::vector<Node*> path, int bytes, bool skipDuplex)
{
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
