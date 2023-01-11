#ifndef ANET_TCPCOMPONENT_H_
#define ANET_TCPCOMPONENT_H_
#include <anet/iocomponent.h>

namespace anet {
class Socket;

const int SHRINK_COUNTDOWN = 10;

class TCPComponent : public IOComponent {
    friend class TCPComponentTF;
    friend class TCPCONNECTIONTF;
    friend class ErrinjectEpollTF;
public:
    TCPComponent(Transport *owner, Socket *socket);
    ~TCPComponent();

    bool init(bool isServer = false);

    Connection *createConnection(IPacketStreamer *streamer,
                                 IServerAdapter *adapter = NULL);
    virtual RECONNErr reconnect();

    /*
     * 关闭
     */
    void close();
    void closeConnection(Connection *conn);
//     void closeSocket();
//     void closeSocketNoLock();

    /*
     * 当有数据可写到时被Transport调用
     * 
     * @return 是否成功, true - 成功, false - 失败。
     */
    bool handleWriteEvent();

    /*
     * 当有数据可读时被Transport调用
     *
     * @return 是否成功, true - 成功, false - 失败。
     */
    bool handleReadEvent();

    bool handleErrorEvent();
    /*
     * 得到connection
     * 
     * @return TCPConnection
     */
    Connection *getConnection() {
        return _connection;
    }

    virtual void dump(std::ostringstream &buf);

    bool checkTimeout(int64_t now);
    bool setState(IOCState state);
    virtual void closeAndSetState();
    bool socketReconnect(int64_t now);
      
protected:
    /*
     * 连接到socket
     */
    bool socketConnect();

    Connection *_connection;
    int64_t _startConnectTime;

private:
    int _shrinkCount;
};
}

#endif /*TCPCOMPONENT_H_*/
