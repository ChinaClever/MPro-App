#ifndef AGENT_CORE_H
#define AGENT_CORE_H

#include "agent_trap.h"



class Agent_Core : public Agent_Trap
{
    explicit Agent_Core(QObject *parent = nullptr);
public:
    static Agent_Core *bulid(QObject *parent = nullptr);


private:
     void startSnmpd();
     void startSnmpdV3();

private:
    sAgentCfg *mCfg;
};

#endif // AGENT_CORE_H
