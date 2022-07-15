/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "agent_core.h"

sAgentCfg Agent_Core::snmpCfg;
Agent_Core::Agent_Core(QObject *parent)
    : Agent_Trap{parent}
{
    mCfg = &snmpCfg;
}

Agent_Core *Agent_Core::bulid(QObject *parent)
{
    static Agent_Core* sington = nullptr;
    if(sington == nullptr) {
        sington = new Agent_Core(parent);
    }
    return sington;
}
