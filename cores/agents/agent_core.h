#ifndef AGENT_CORE_H
#define AGENT_CORE_H

#include "agent_trap.h"

struct sAgentCfg {
    QString trap1;
    QString trap2;

    int enV3;
    QString usr;
    QString pwd;
    QString key;
};

class Agent_Core : public Agent_Trap
{
    explicit Agent_Core(QObject *parent = nullptr);
public:
    static Agent_Core *bulid(QObject *parent = nullptr);
    static sAgentCfg snmpCfg;

private:
     void startSnmpd();
     void startSnmpdV3();

private:
    sAgentCfg *mCfg;
};

#endif // AGENT_CORE_H
