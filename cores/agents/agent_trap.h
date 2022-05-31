#ifndef AGENT_TRAP_H
#define AGENT_TRAP_H

#include "agent_set.h"

class Agent_Trap : public Agent_Set
{
    Q_OBJECT
public:
    explicit Agent_Trap(QObject *parent = nullptr);

private slots:
    void initTrapSlot();
    void alarmSlot(sDataItem &index, uchar value);

private:
    QSNMPOid  mModuleOid;
};

#endif // AGENT_TRAP_H
