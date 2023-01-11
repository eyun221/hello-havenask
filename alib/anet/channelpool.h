#ifndef ANET_CHANNEL_POOL_H_
#define ANET_CHANNEL_POOL_H_
#include <stdint.h>
#include <sys/types.h>
#include <stdlib.h>
#include <anet/atomic.h>
#include <anet/threadmutex.h>
#include <anet/channel.h>
#include <anet/common.h>
#include <list>
#include <tr1/unordered_map>

namespace anet {
const size_t CHANNEL_CLUSTER_SIZE = 64lu;

class ChannelPool {
public:
    static const uint64_t AUTO_CHANNEL_ID_MAX;
    static const uint64_t HTTP_CHANNEL_ID;
    static const uint64_t ADMIN_CHANNEL_ID;
public:
    ChannelPool();
    ~ChannelPool();

public:
    /**
     * Allocate a new channel.
     *
     * @param chid The desired channel id for the channel just allocated. If
     * chid is less than or equal to AUTO_CHANNEL_ID_MAX, the channel id of
     * the new channel will be generated automatically, which is larger than
     * 0 and less than AUTO_CHANNEL_ID_MAX. if chid is greater than
     * AUTO_CHANNEL_ID_MAX,  the channel id of the new channel will be chid.
     */
    Channel* allocChannel(uint64_t chid = 0);
    bool freeChannel(Channel *channel);
    Channel* findChannel(uint64_t id);
    Channel* getTimeoutList(int64_t now);
    bool appendFreeList(Channel *addList);
    void addToWaitingList(Channel *channel);

    size_t getUseListCount() {
        return atomic_read(&_useCount);
    }

private:
    inline void addUseCountByOne()     {
        atomic_add(1, &_useCount);
    }

    inline void subUseCountByOne()     {
        atomic_sub(1,&_useCount);
    }


    uint64_t createChannelId(uint64_t chid);
    void freeWaitingChannel(Channel *channel);
    void recycleChannel(Channel *channel);

private:
    std::tr1::unordered_map<uint64_t, Channel*> _waitReplyMap;
    std::list<Channel*> _clusterList;

    ThreadMutex _freeListLock;
    ThreadMutex _waitReplyLock;

    Channel *_freeListHead;
    Channel *_freeListTail;

    Channel *_waitReplyHead;
    Channel *_waitReplyTail;
    atomic_t _useCount;

    static atomic64_t _globalChannelId;

    friend class ChannelPoolTF;
    friend class ConnectionTF;
};

}

#endif /*CHANNEL_POOL_H_*/
