#ifndef MESSAGESTATS_H
#define MESSAGESTATS_H

#include <QString>

struct MessageLogEntry {
    int srcId;
    int dstId;
    QString mode;
    int messageSize;
    int packetSize;
    int totalPackets;
    int serviceBytes;
    double totalTime;
    QString pathString;
};

#endif
