#ifndef DAEMONS_H
#define DAEMONS_H

#define SHM_KEY	0x5105

#include "procstart.h"

class Daemons : public ProcStart
{
    Daemons();
public:
    static Daemons *bulid();
    void workDown();

private:
    void initFun();
    void start_proc();
    void init_share_mem();
    bool resetProc(sRunTime &proc, const QString &name);
    bool procRunStatus(sRunTime &proc, const QString &name);

private:
    sProcState *mProcs;
};

#endif // DAEMONS_H
