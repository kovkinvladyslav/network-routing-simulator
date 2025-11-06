#ifndef CHANNELPROPERTIES_H
#define CHANNELPROPERTIES_H

enum class ChannelType { Duplex, HalfDuplex };
enum class ChannelMode { Normal, Satellite };

struct ChannelProperties {
    int weight;
    ChannelType type;
    ChannelMode mode;
    bool active = true;
    double errorProb = 0.1;
};

#endif // CHANNELPROPERTIES_H
