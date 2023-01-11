/**
 * File name: databufferserializable.h
 * Author: lizhang
 * Create time: 2010-11-07 06:11:52
 * $Id$
 * 
 * Description: ***add description here***
 * 
 */

#ifndef ANET_DATABUFFERSERIALIZABLE_H_
#define ANET_DATABUFFERSERIALIZABLE_H_

#include <anet/databuffer.h>

namespace anet {
class DataBufferSerializable
{
public:
    DataBufferSerializable() {}
    virtual ~DataBufferSerializable() {}
    
    virtual bool serialize(DataBuffer *inputBuffer) const = 0;
    
    /**
     * deserialze from inputBuffer. if length is greater then 0, deserialze()
     * should consume length bytes from inputBuffer, Otherwise, the the data
     * in the inputBuffer either self-delimited or limited by the boundary of
     * inputBuffer
     */
    virtual bool deserialize(DataBuffer *inputBuffer, int length = 0) = 0;
    virtual void free() {delete this;}

    virtual int64_t getSpaceUsed() { return 0; };
};

}/*end namespace anet*/
#endif /*ANET_DATABUFFERSERIALIZABLE_H_*/
