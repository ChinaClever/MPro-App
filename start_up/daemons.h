#ifndef DAEMONS_H
#define DAEMONS_H

#define SHM_KEY	0x5105
#define SUPPORT_C 1
#include "/home/lzy/work/NPDU/cores/commons/cms/datapacket.h"
#include "procusb.h"

class Daemons : public ProcUsb
{
    Daemons();
public:
    static Daemons *bulid();
    void workDown();

protected:
    void coreRunStatus();
    void lcdRunStatus();

private:
    void start_proc();
    void init_share_mem();
    bool procRunStatus(sRunTime *proc);

private:
    sProcState *mProcs;
};

#endif // DAEMONS_H
