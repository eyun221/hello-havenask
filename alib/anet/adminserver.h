#ifndef ADMIN_SERVER_H_
#define ADMIN_SERVER_H_
#include <anet/anet.h>
#include <anet/runnable.h>
#include <anet/thread.h>
#include <anet/admincmds.h>
#include <sys/types.h>
#include <sys/syscall.h>

BEGIN_ANET_NS();

pid_t gettid();

/* Defines the prefix for admin file. The final look should like admin-<pid>.*/ 
#define ADMIN_FILE_PREFIX "admin-"

/**
 * Admin server implementation.
 */
class AdminServer : public IServerAdapter, public Runnable {
public:
    AdminServer(std::string spec = "") : _spec(spec) { 
        _threadCreated = false; _pid = 0;
       /* Init ANET subsys Cmd Table */
       initAnetCmds(cmdTable);
    }

    virtual ~AdminServer() {}

    /* Run server in a seperate thread. */
    void runInThread();
    pid_t getServerId(){
        return _pid;
    }

    void stop() 
    {
        _transport.stop();
        _thread.join();
        _threadCreated = false;
        _pid = 0;
        _transport.wait();
    }

    IPacketHandler::HPRetCode handlePacket(Connection *, Packet *);

    /* Registration of new commands. */
    SubSysEntry * registerSubSys(std::string & name) {
        return cmdTable.GetOrAddSubSys(name);
    }
    bool registerCmd(std::string subsys, std::string cmdName, std::string cmdUsage, ADMIN_CB cb) {
        return cmdTable.AddCmd(subsys, cmdName, cmdUsage, cb);
    }

private:
    /* Runnable interface. */
    void run(Thread *thread, void *arg);

    /* Start server in current thread. This function will block the caller in 
     * transport.run().
     * End user should not use it because of the above reason. So make it private
     * for now. */
    void start();

private:
    std::string _spec;
    volatile bool _threadCreated;
    Transport _transport;
    pid_t _pid;
    Thread _thread;
    CMDTable cmdTable;

};

END_ANET_NS();
#endif
