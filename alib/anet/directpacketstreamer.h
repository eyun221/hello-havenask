#ifndef ANET_DIRECT_PACKET_STREAMER_H_
#define ANET_DIRECT_PACKET_STREAMER_H_

#include <anet/socket.h>
#include <anet/directpacket.h>
#include <anet/defaultpacketstreamer.h>
#include <anet/directstreamingcontext.h>
#include <anet/channelpool.h>

namespace anet {

constexpr int64_t DIRECT_WRITE_PAYLOAD_THRESHOLD = 8 * 1024;

class Packet;
class DirectPacketFactory : public IPacketFactory {
public:
    ~DirectPacketFactory() override {}
    Packet *createPacket(int pcode) override;
    DirectPacket *createDirectPacket(int pcode);
};

class DirectPacketStreamer : public DefaultPacketStreamer {
    friend class DirectPacketStreamerTF;

public:
    DirectPacketStreamer();
    ~DirectPacketStreamer();
    StreamingContext *createContext() override;
    bool encode(Packet *packet, DataBuffer *output, DirectPayload &payloadToWrite);
    bool processData(DataBuffer *dataBuffer, StreamingContext *context) override;
    bool readPayload(DataBuffer *input, Socket *socket, ChannelPool *channelPool,
                     DirectStreamingContext *context, int &socketReadLen);

private:
    bool getPacketInfo(DataBuffer *input, PacketHeader *header, DirectPacketHeader *directHeader,
                       bool *broken);
    const DirectPlaceholder &getPlaceholderFromChannel(ChannelPool *channelPool,
                                                       uint64_t channelId);
    Packet *decode(DataBuffer *input, PacketHeader *header) override;

private:
    DirectPacketFactory _directPacketFactory;
};

}  // namespace anet

#endif  // ANET_DIRECT_PACKET_STREAMER_H_
