#ifndef ANET_IOCOMPONENT_H_
#define ANET_IOCOMPONENT_H_

#include <anet/atomic.h>
#include <anet/threadmutex.h>
#include <anet/socket.h>
#include <anet/timeutil.h>

#include <sstream>

namespace anet {
const int MAX_RECONNECTING_TIMES = 30;
const int64_t MAX_IDLE_TIME_IN_MICROSECONDS = 900000000;//15 minutes
const int MAX_IDLE_TIME_IN_MILLSECONDS = 900000;//15 minutes
const int DEFAULT_POSTPACKET_TIMEOUT = 5000;//5s
const int64_t RECONNECTING_INTERVAL = 1000000;//1 seconds

class Connection;
class SocketEvent;
class IPacketStreamer;
class IServerAdapter;
class Socket;
class Transport;
class IoWorker;


/* Error code for manual reconnection. */
typedef enum {
    ANET_RECONN_OK = 0,
    ANET_RECONN_CONFLICT = -1,
    ANET_RECONN_CONNECTED = -2,
    ANET_RECONN_CLOSED = -3,
    ANET_RECONN_SOCKFAILED = 1
} RECONNErr;

enum IocType {
    IOC_BASE, 
    IOC_TCPACCEPTOR, 
    IOC_TCPCOMPONENT, 
};

class IOComponent {
    friend class Transport;
    friend class TransportTF;
    friend class IoWorker;
    friend class TCPComponentTF;
    friend class ConnecionTF;
    friend class TCPCONNECTIONTF;
public:
    enum IOCState {
        ANET_TO_BE_CONNECTING = 1,
        ANET_CONNECTING,
        ANET_CONNECTED,
        ANET_CLOSING,
        ANET_CLOSED,
    };

public:
    /**
     * @param owner: pointer of a transport object
     * @param socket pointer of a socket object
     */
    IOComponent(Transport *owner, Socket *socket);

    virtual ~IOComponent();

    /**
     * @param isServer indicate if this component is used as server
     * @return 
     */
    virtual bool init(bool isServer = false) = 0;

    /**
     * create a connection based on this iocomponent;
     * @param streamer:   streamer used to decode/encode data from/to
     * input/output buffer
     * @param serverAdapter:  user defined adapter to serve requests
     */ 
    virtual Connection *createConnection(IPacketStreamer *streamer,
            IServerAdapter *adapter = NULL) {
        return NULL;
    }

    /**
     * connect or reconnect to the target if it is not in connected state.
     */
    virtual RECONNErr reconnect();

    /**
     * ??????
     */
    virtual void close() {}

    virtual void closeSocket();
    virtual void closeSocketNoLock();

    virtual void closeConnection(Connection *);

    /**
     * ???????????????????????????Transport??????
     * 
     * @return ????????????, true - ??????, false - ?????????
     */
    virtual bool handleWriteEvent() = 0;

    /**
     * ????????????????????????Transport??????
     * 
     * @return ????????????, true - ??????, false - ?????????
     */
    virtual bool handleReadEvent() = 0;

    /***
     * called when error occured
     */
    virtual bool handleErrorEvent() = 0;

    /**
     * ????????????
     * 
     * @param    now ????????????(??????us)
     * @return when return false, we should remove this iocomponent
     * from timeout thread's checking list
     */
    virtual bool checkTimeout(int64_t now) = 0;

    /**
     * ??????socket??????
     * 
     * @return Socket
     */
    inline Socket *getSocket() {
        return _socket;
    }

    /**
     * ??????SocketEvent
     */
    void setSocketEvent(SocketEvent *socketEvent) ;

    /**
     * ????????????
     *
     * @param on ???????????????
     */
    void enableRead(bool on) ;

    /**
     * ????????????
     *
     * @param on ???????????????
     */
    void enableWrite(bool on) ;

    inline void updateUseTime(int64_t now) {
        _lastUseTime = now;
    }

    /**
     * ??????????????????
     */
    void addRef() ;

    /**
     * ?????????????????? 
     */
    void subRef();

    /**
     * ?????????????????? 
     */
    int getRef();
    /**
     * ??????????????????, ????????????????????????
     */
    inline bool isConnectState() {
        return getState() < ANET_CLOSING;
    }

    inline bool isClosed() {
        return getState() >= ANET_CLOSING;
    }

    /**
     * ??????????????????
     */
    inline void setAutoReconn(bool on) {
        /***only attempts to reconnect 30 times at most*/
        _autoReconn = (_isServer || !on) ? 0 : MAX_RECONNECTING_TIMES;
    }
 
    /**
     * ?????????????????? 
     */
    inline bool isAutoReconn() {
        return (_autoReconn && !_isServer);
    }

    /***
     * Is this IOComponent referenced by read/write thread of transport
     */
    bool isReferencedByReadWriteThread() {
        return _refByRreadWriteThread;
    }

    /***
     * Set _refByRreadWriteThread flag
     */
    inline void referencedByReadWriteThread(bool flag) {
        _refByRreadWriteThread = flag;
    }

    void lock() ;
    void unlock();
    
    inline IOCState getState() {
        return _state;
    }

    virtual bool setState(IOCState state) ;
    virtual void closeAndSetState() {}
    
    void setMaxIdleTime(int maxIdleTimeInMillseconds) {
        if (maxIdleTimeInMillseconds == 0) {
            _maxIdleTimeInMicroseconds = MAX_IDLE_TIME_IN_MICROSECONDS;
        } else if (maxIdleTimeInMillseconds < 0) {
            _maxIdleTimeInMicroseconds = TimeUtil::MAX;
        } else {
            _maxIdleTimeInMicroseconds = (int64_t)maxIdleTimeInMillseconds * 1000;
        }
    }

    virtual void dump(std::ostringstream &buf) {
        char timestr[32];
        TimeUtil::getTimeStr(_lastUseTime, timestr);

        buf << "------IOC State------\n";
        buf << "IOC Address: " << this << "\t" << "Owner transport: " << _owner 
            << "\t" << "Socket: " << _socket << std::endl;
        buf << "IOC State: " << _state << "\t" << "Ref Count: " 
            << atomic_read(&_refcount) << "\t" << "Last Use: " 
            << _lastUseTime << "-" << std::string(timestr);
        buf << "Enable Read: " << _enableRead << "\t" << "Enable Write: " 
            << _enableWrite << "\t" << "Ref By RW Thread: " 
            << _refByRreadWriteThread << std::endl;
        buf << "Max Idle Time: " << _maxIdleTimeInMicroseconds << "us" << std::endl;
        buf << "Prev IOC: " << _prev << "\t" << "Next IOC: " << _next << std::endl;

        /* start dumping socket state. */
        _socket->dump("    ", buf);
    }

    /*
     * get listen port for a compontent
     */
    char* getLocalAddrStr(char *dest, int len);
    void shutdownSocket();

    IocType getType() const {
        return _type;
    }

protected:
    /**
     * Transport distribute ioworker based on socket fd
     * So this function should be invoked after acquiring a fd
     *
     * Init _belongedWorker and _socketEvent
     */
    void initialize();

protected:
    Transport *_owner;
    Socket *_socket;    // ??????Socket???????????????
    SocketEvent *_socketEvent;
    IOCState _state;         // ????????????
    atomic_t _refcount; // ????????????
    int _autoReconn;   // ????????????
    bool _isServer;     // ?????????????????????
    int64_t _lastUseTime;   // ???????????????????????????    
    bool _enableRead;
    bool _enableWrite;
    bool _refByRreadWriteThread;
    int64_t _maxIdleTimeInMicroseconds; 
    ThreadMutex _mutex;
    bool _isSocketInEpoll;
    ThreadMutex _socketMutex;
    IocType _type;
    IoWorker *_belongedWorker;
private:
    IOComponent *_prev; // ????????????
    IOComponent *_next; // ????????????
};
}

#endif /*IOCOMPONENT_H_*/
