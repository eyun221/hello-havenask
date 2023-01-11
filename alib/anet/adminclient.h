#ifndef ADMIN_CLIENT_H
#define ADMIN_CLIENT_H

#include <anet/anet.h>
#include <anet/admincmds.h>

#define MAX_CMD_BUF 1024
namespace anet{

class AdminClient
{
public:
    AdminClient(Transport *transport, char *spec);
    ~AdminClient();
    int start(IPacketHandler *handler, void *args, char *cmd, 
              int paramCount, char **params);

private:
    char *_spec;
    Transport * _transport;
    AdvanceDefaultPacketFactory _factory ;
    DefaultPacketStreamer _streamer;
};


}
#endif
