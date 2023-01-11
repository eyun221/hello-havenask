#ifndef ANET_CHANNEL_H_
#define ANET_CHANNEL_H_
#include <stdint.h>
#include <anet/ipackethandler.h>
#include <anet/common.h>
#include <anet/directplaceholder.h>
BEGIN_ANET_NS();

class Channel {
    friend class ChannelPool;
    friend class ChannelTF;
    friend class ChannelPoolTF;
public:
    Channel();
    void init(uint64_t id);
    void setId(uint64_t id);
    uint64_t getId();

    void setArgs(void *args);
    void *getArgs();

    void setHandler(IPacketHandler *handler);
    IPacketHandler *getHandler();

    const DirectPlaceholder &getPlaceholder() const { return _placeholder; }
    void setPlaceholder(const DirectPlaceholder &placeholder) { _placeholder = placeholder; }

    void setExpireTime(int64_t expireTime);
    int64_t getExpireTime() {
        return _expireTime;
    }

    Channel *getNext() {
        return _next;
    }
    Channel *getPrev() {
        return _prev;
    }

private:
    uint64_t _id;      // channel id
    void *_args;    // call back argument
    IPacketHandler *_handler;
    DirectPlaceholder _placeholder;
    int64_t _expireTime; 

private:
    Channel *_prev;
    Channel *_next;
};
END_ANET_NS();

#endif /*CONNECTION_H_*/
