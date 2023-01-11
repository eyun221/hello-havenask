#ifndef _ANET_E2E_QOS_H_
#define _ANET_E2E_QOS_H_
#include <stdint.h>
namespace anet {
/**
 * 流控
 */

class E2eQos
{
public:
    E2eQos();
    ~E2eQos();
    int AddGroup(int sk, uint32_t gid, uint64_t jobid=0, uint32_t insid=0);
    int RemoveGroup(int sk);
private:
    int (*_qos_join_group)(int sk, unsigned long job, unsigned inst, unsigned group);
    int (*_qos_quit_group)(int sk);
    void *handle;
};

extern E2eQos _ge2eQos;
}//anet

#endif
