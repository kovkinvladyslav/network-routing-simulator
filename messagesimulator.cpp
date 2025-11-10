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

    std::vector<Node*> path = graph->getShortestPath(src, dst, RouteMetric::EffectiveCost);

    if (path.size() < 2) {
        result.delivered = false;
        return result;
    }

    for (int i = 0; i < result.totalPackets; i++) {

        for (int j = 0; j + 1 < (int)path.size(); j++) {
            Node* a = path[j];
            Node* b = path[j + 1];
            const auto& props = a->get_adj().at(b);

            result.totalTime += graph->computeTransmissionTime(props, packetSizeBytes);

            if (props.errorProb > 0.0)
                result.retransmissions += (props.errorProb / (1.0 - props.errorProb));
        }
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

    result.totalTime += path.size() * 5.0;

    result.totalPackets = std::ceil((double)messageSizeBytes / packetSizeBytes);

    for (int i = 0; i < result.totalPackets; i++) {

        for (int j = 0; j + 1 < (int)path.size(); j++) {

            Node* a = path[j];
            Node* b = path[j + 1];
            const auto& props = a->get_adj().at(b);

            result.totalTime += graph->computeTransmissionTime(props, packetSizeBytes);

            if (props.errorProb > 0.0)
                result.retransmissions += (props.errorProb / (1.0 - props.errorProb));
        }
    }

    result.delivered = true;
    result.lastPath = path;

    return result;
}
