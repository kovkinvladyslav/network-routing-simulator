#ifndef CHANNELPROPERTIES_H
#define CHANNELPROPERTIES_H

enum class ChannelType { Duplex, HalfDuplex };
enum class ChannelMode { Normal, Satellite };

struct ChannelProperties {
    int weight = 1;
    ChannelType type = ChannelType::Duplex;
    ChannelMode mode = ChannelMode::Normal;
    bool active = true;
    double errorProb = 0.1;
};

#endif // CHANNELPROPERTIES_H
