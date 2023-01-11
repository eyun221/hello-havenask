#ifndef ANET_DIRECTSTREAMINGCONTEXT_H_
#define ANET_DIRECTSTREAMINGCONTEXT_H_

#include <anet/directpacket.h>
#include <anet/streamingcontext.h>

namespace anet {
class DirectStreamingContext : public StreamingContext {
public:
    DirectStreamingContext() {}
    virtual ~DirectStreamingContext() override {}

    DirectPacket *getDirectPacket() { return static_cast<DirectPacket *>(_packet); }
    DirectPacket *stealDirectPacket() { return static_cast<DirectPacket *>(stealPacket()); }

    bool isPayloadCompleted() const { return _payloadCompleted; }
    void setPayloadCompleted(bool payloadCompleted) { _payloadCompleted = payloadCompleted; }

    size_t getPayloadReadOffset() const { return _payloadReadOffset; }
    void setPayloadReadOffset(size_t payloadReadOffset) { _payloadReadOffset = payloadReadOffset; }

    void reset() override {
        _payloadCompleted = true;
        _payloadReadOffset = 0UL;
        StreamingContext::reset();
    }

private:
    bool _payloadCompleted = true;
    size_t _payloadReadOffset = 0UL;
};

}  // namespace anet
#endif  // ANET_DIRECTSTREAMINGCONTEXT_H_
