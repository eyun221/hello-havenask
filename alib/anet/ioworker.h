#ifndef ANET_IO_WORKER_H
#define ANET_IO_WORKER_H

#include <vector>

#include <anet/epollsocketevent.h>
#include <anet/iocomponent.h>
#include <anet/socket.h>
#include <anet/thread.h>
#include <anet/transport.h>

namespace anet {

/**
 * Class IoWorker is response for watch on a set of file descriptor and 
 * dispatch the events on them.
 * There are two mode to run an IoWorker instance. a) Run it with a standalone
 * thread. In this case, call IoWorker::start(), then a new thread will be 
 * created to do the io staff work. b) Call IoWorker::step() directly. Then
 * the calling thread is response for calling the epoll functions and handle
 * the read/write events.
 */
class IoWorker : public Runnable {
    friend class ConnectionTF;
    friend class TCPCONNECTIONTF;
    friend class TCPComponentIdleTimeTF;
    friend class TransportTF;
    friend class IoWorkerTF;
public:
    IoWorker();

    ~IoWorker();

    /**
     * @param promotePriority promote the proproty of ANET thread
     * @return Return true if start success, else return false.
     */
    bool start(bool promotePriority = false);

    /**
     * Stop ANET. It will stop all threads of ANET.
     * In this function, _stopMutex will be locked.
     *
     * @return Return true if stop success, else return false. 
     */
    bool stop();

    /**
     * Waiting for read/write thread exit.
     *
     * @return Return true if wait success, else return false.
     */
    bool wait();

    /**
     * Start thread.
     *
     * @param arg: args for thread
     */
    void run(Thread *thread, void *arg);

    void eventLoop();

    /**
     * Run iteration in single thread. In this function, 
     * eventIteration and timeoutIteration will be called.
     *
     * @param timeStamp: execute time
     */
    void step(int64_t timeStamp);

    void postCommand(const Transport::CommandType type, IOComponent * ioc);

    SocketEvent* getSocketEvent();
    void closeComponents();
    int  dump(std::ostringstream &buf);
    void getTcpConnStats(std::vector<ConnStat> &connStats);

    void setName(const char *name);
private:
    /**
     * process Command in queue _commands;
     */ 
    void processCommands();
    void addComponent(IOComponent *ioc);
    void removeComponent(IOComponent *ioc);

private:
    // mutex for _iocList and _commands
    ThreadMutex _mutex;
    ThreadMutex _stopMutex;
    EPollSocketEvent _socketEvent;   
    Thread _ioThread;      
    bool _stop;              // stopping flag
    bool _started;
    bool _promotePriority;
    IOComponent *_iocListHead, *_iocListTail;   // IOComponent list
    std::vector<Transport::TransportCommand> _commands;
public:
    static __thread int64_t _loopTime;
};

}

#endif // ANET_IO_WORKER_H
