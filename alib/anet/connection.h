#ifndef ANET_CONNECTION_H_
#define ANET_CONNECTION_H_

#define READ_WRITE_SIZE 8192
#include <sstream>
#include <anet/orderedpacketqueue.h>
#include <anet/threadcond.h>
#include <anet/channelpool.h>
#include <anet/ipacketstreamer.h>
#include <anet/iconnection.h>
#include <anet/iocomponent.h>

namespace anet {
class IPacketHandler;
class Packet;
class IOComponent;
class IServerStreamer;
class IServerAdapter;
class Socket;
struct ConnStat {
    std::string addr;
    int64_t packPosted;
    int64_t packHandled;
    int64_t bytesWrited;
    int64_t bytesRead;
};

class Connection : public IConnection
{
    friend class ConnectionTF;
    friend class TCPCONNECTIONTF;
    friend class HTTPConnectionTF;
public:
    /* Callback state used by R/W thread
     *
     * CB_RUNNING will be set just before R/W thread transfer control to user's callback,
     * and reset to CB_IDLE immediately after the callback finishes.
     */
    enum RWCBState {
        RW_CB_IDLE, 
        RW_CB_RUNNING, 
    };

    Connection(Socket *socket, IPacketStreamer *streamer, IServerAdapter *serverAdapter);

    virtual ~Connection();

    void setServer(bool isServer) {
        _isServer = isServer;
    }

    bool isServer() {
        return _isServer;
    }

    void setIOComponent(IOComponent *ioc) {
        _iocomponent = ioc;
    }

    IOComponent *getIOComponent() {
        return _iocomponent;
    }

    void setDefaultPacketHandler(IPacketHandler *ph) {
        _defaultPacketHandler = ph;
    }

    IPacketHandler *getDefaultPacketHandler() {
        return _defaultPacketHandler;
    }

    /**
     * async interface
     *
     * @param packet 
     * @param packetHandler 
     * @param args
     * @param block whether block if output queue is full.
     */
    bool postPacket(Packet *packet, IPacketHandler *packetHandler = NULL,
            void *args = NULL, bool block = false);

    /**
     * Send a request packet to server, then waiting for the replay packet.
     * If the return value is NULL, caller is responsible for freeing the
     * request packet; If the return value is NOT NULL, ANet will free the
     * request packet, and caller is responsible for freeing the reply packet.
     *
     * @param packet the request to be sent to server
     * @param block whether block if output queue is full.
     * @return the value returned is a pointer to a reply packet received
     * from server, or NULL if fail to send the reply.
     */
    Packet *sendPacket(Packet *packet, bool block = false);

    bool handlePacket(Packet *header);

    int64_t getPacketPostedCount() {return _packetPostedCount;}
    int64_t getPacketHandledCount() {return _packetHandledCount;}
    int64_t getPacketTimeoutCount() {return _packetTimeoutCount;}

    bool checkTimeout(int64_t now);

    void closeHook();
    void openHook();

    virtual bool writeData() = 0;

    virtual bool readData() = 0;

    virtual void setWriteFinishClose(bool v) {
    }

    virtual void setWriteFinishShutdown(bool v) {
    }

    void setQueueTimeout(int queueTimeout) {
        _queueTimeout = queueTimeout;
    }

    virtual void clearOutputBuffer() {
        ;
    }

    virtual void clearInputBuffer() {
        ;
    }
    
    virtual int64_t getInputBufferSpaceAllocated() { return 0; }
    virtual int64_t getOutputBufferSpaceAllocated() { return 0; }
    int64_t getDataWriteBytes() { return _dataWriteBytes; }
    int64_t getDataReadBytes() { return _dataReadBytes; }
    int64_t getOutputQueueSpaceUsed() { 
        return atomic_read(&_outputQueueSpaceUsed); 
    }
    int getOutputQueueSize() { 
        return atomic_read(&_outputQueueSize); 
    };

    bool setQueueLimit(size_t limit); 
    size_t getQueueLimit();

    virtual void close();
    virtual void addRef();
    virtual void subRef();
    virtual int getRef();

    virtual bool isClosed();
    virtual bool isConnected();

    void wakeUpSender();
    void beforeCallback();
    void afterCallback();

    void resetContext(); 
    StreamingContext* getContext() {
        return _context;
    }

    void setServer(IServerAdapter *serverAdapter) {
        _serverAdapter = serverAdapter;
    }

    int setPriority(CONNPRIORITY priority);

    virtual int setQosGroup(uint64_t jobid, uint32_t instanceid, uint32_t groupid);
    virtual int setQosGroup();

    uint32_t getQosGroup();

    CONNPRIORITY getPriority();

    bool setKeepAliveParameter(int idleTime, int keepInterval, int cnt);
    
    /* manually reconnect to the target, will always return error when
     * auto reconnection is enabled. */
    RECONNErr reconnect();

    char* getLocalAddr(char *dest, int len);
    char* getIpAndPortAddr(char *dest, int len);
    /* get local bind address for a connection
     */
    bool getSockAddr(sockaddr_in &addr);

    Socket* getSocket() {
        return _socket;
    }

    void dump(const char *lead, std::ostringstream &buf) {
        std::string leading(lead);

       buf << leading << "Connection Addr: " << this 
           << "\t" << "Is Server: " << _isServer 
           << "\t" << "Closed: " << _closed << std::endl; 
       buf << leading << "Socket: " << _socket
           << "\t" << "Priority: " << getPriority() 
           << "\t" << "DefaultPacketHandler: " << _defaultPacketHandler << std::endl;
       buf << leading << "Streamer: " << _streamer 
           << "\t" << "ServerAdapter: " << _serverAdapter << std::endl;
       buf << leading << "ChannelPool: " << &_channelPool << std::endl;
       buf << leading << "Queue Timeout: " << _queueTimeout << "ms" 
           << "\t" << "Queue Limit: " << _queueLimit << std::endl;
       buf << leading << "Waiting Threads: " << _waitingThreads << std::endl;
       buf << leading << "Packet Posted: "   << _packetPostedCount 
           << "\t" << "Packet Handled: " << _packetHandledCount 
           << "\t" << "Packet Timeout: " << _packetTimeoutCount << std::endl;
       buf << leading << "Bytes Write: " << _dataWriteBytes
           << "\t" << "Bytes Read: "     << _dataReadBytes << std::endl;
       buf << leading << "Output Q Size: " << atomic_read(&_outputQueueSize) 
           << "\t" << "Output Q Memory: " << atomic_read(&_outputQueueSpaceUsed) << std::endl;
    }

    RWCBState getRWCBState() {
        return _rwCbState;
    }
    
protected:
    void updateQueueStatus(Packet *packet, bool pushOrPopFlag);

protected:
    IPacketHandler *_defaultPacketHandler;
    bool _isServer;                         
    IOComponent *_iocomponent;
    Socket *_socket;                        
    IPacketStreamer *_streamer;             // Packet Streamer
    IServerAdapter *_serverAdapter;         // Server Adapter

    OrderedPacketQueue  _outputQueue;              // Output Queue
    OrderedPacketQueue _myQueue;                // temporary queue used in write data
    ThreadCond _outputCond;                 // output condition
    ChannelPool _channelPool;
    int _queueTimeout;                      // tiemout spesification (millseconds)
    size_t _queueLimit;                     // max pending set packet allowed
    int _waitingThreads;
    bool _closed;
    StreamingContext *_context;

    int64_t _packetPostedCount;
    int64_t _packetHandledCount;
    int64_t _packetTimeoutCount;
    int64_t _dataWriteBytes;
    int64_t _dataReadBytes;
    atomic_t _outputQueueSize;
    atomic64_t _outputQueueSpaceUsed;
    /* Priority of this connection. */

    ThreadMutex _addrMutex;
    char* _socketAddr;
    size_t _socketAddrLen;
    uint64_t _jobId;
    uint32_t _insId;
    uint32_t _qosId;
    int64_t _lasttime;

private:
    RWCBState _rwCbState;
};
}

#endif /*CONNECTION_H_*/

