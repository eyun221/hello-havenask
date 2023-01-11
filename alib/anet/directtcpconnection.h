#ifndef ANET_DIRECTTCPCONNECTION_H_
#define ANET_DIRECTTCPCONNECTION_H_

#include <anet/tcpconnection.h>
#include <anet/directpacket.h>

namespace anet {

class DirectPacketStreamer;
class DirectStreamingContext;

class DirectTCPConnection : public TCPConnection {
    friend class DirectTCPConnectionTF;

public:
    DirectTCPConnection(Socket *socket, IPacketStreamer *streamer, IServerAdapter *serverAdapter);
    ~DirectTCPConnection();
    bool writeData() override;
    bool readData() override;

private:
    DirectPacketStreamer *_directStreamer;
    DirectStreamingContext *_directContext;
    size_t _payloadLeftToWrite = 0UL;
    DirectPayload _writingPayload;
    DirectPacket *_writingPacket = nullptr;
};

}  // namespace anet

#endif  // ANET_DIRECTTCPCONNECTION_H_
