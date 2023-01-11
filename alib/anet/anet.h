#ifndef ANET_ANET_H
#define ANET_ANET_H
#include <anet/transport.h>

#include <anet/packet.h>
#include <anet/controlpacket.h>
#include <anet/ipackethandler.h>
#include <anet/iserveradapter.h>

#include <anet/iocomponent.h>
#include <anet/connection.h>

#include <anet/defaultpacket.h>
#include <anet/defaultpacketfactory.h>
#include <anet/defaultpacketstreamer.h>


#include <anet/httppacket.h>
#include <anet/httppacketfactory.h>
#include <anet/httpstreamer.h>

#include <anet/httppacket.h>
#include <anet/httppacketfactory.h>
#include <anet/httpstreamer.h>

#include <anet/databuffer.h>
#include <anet/ipacketfactory.h>
#include <anet/ipacketstreamer.h>
#include <anet/streamingcontext.h>
#include <anet/timeutil.h>
#include <anet/delaydecodepacket.h>
#include <anet/delaydecodepacketfactory.h>
#include <anet/databufferserializable.h>

#include <anet/advancepacket.h>
#include <anet/advancepacketfactory.h>
#include <anet/connectionpriority.h>

//DO NOT export interfaces about logging implicitly
//#include <anet/log.h>

/**legacy http related header files*/
#include <anet/httprequestpacket.h>
#include <anet/httppacketstreamer.h>
#include <anet/httpresponsepacket.h>

// for direct packet
#include <anet/directpacket.h>
#include <anet/directpacketstreamer.h>
#include <anet/directtcpconnection.h>
#include <anet/directstreamingcontext.h>

#define BEGIN_ANET_NS() namespace anet {
#define END_ANET_NS() }
#define USE_ANET_NS() using namespace anet;

BEGIN_ANET_NS()

/* Redefine advance packet type to make it easier to use. */
typedef AdvancePacket<DelayDecodePacket> AdvanceDelayDecodePacket;
typedef AdvancePacket<DefaultPacket> AdvanceDefaultPacket;
typedef AdvancePacketFactory<DelayDecodePacket> AdvanceDelayDecodePacketFactory;
typedef AdvancePacketFactory<DefaultPacket> AdvanceDefaultPacketFactory;

END_ANET_NS()

#define ANET_VERSION 040000
#endif/*End of ANET_ANET_H*/

