#ifndef AGENT_CORE_H
#define AGENT_CORE_H

#include "agent_trap.h"

class Agent_Core : public Agent_Trap
{
    Q_OBJECT
    explicit Agent_Core(QObject *parent = nullptr);
public:
    static Agent_Core *bulid(QObject *parent = nullptr);

};

#endif // AGENT_CORE_H
