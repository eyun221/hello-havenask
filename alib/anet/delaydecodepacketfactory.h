/**
 * File name: delaydecodepacketfactory.h
 * Author: lizhang
 * Create time: 2010-11-07 18:34:30
 * $Id$
 * 
 * Description: ***add description here***
 * 
 */

#ifndef ANET_DELAYDECODEPACKETFACTORY_H_
#define ANET_DELAYDECODEPACKETFACTORY_H_

#include <anet/ipacketfactory.h>

namespace anet {
class DelayDecodePacketFactory : public IPacketFactory
{
public:
    DelayDecodePacketFactory();

    ~DelayDecodePacketFactory();

    virtual Packet* createPacket(int pcode);
};

}/*end namespace anet*/
#endif /*ANET_DELAYDECODEPACKETFACTORY_H_*/
