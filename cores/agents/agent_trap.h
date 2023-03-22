#ifndef AGENT_TRAP_H
#define AGENT_TRAP_H

#include "agent_set.h"

class Agent_Trap : public Agent_Set
{
public:
    explicit Agent_Trap(QObject *parent = nullptr);

private:
    void timeoutDone();
    void initTrapSlot();
    void alarmSlot(const sDataItem &index, uchar value);
    void sendTrap(const QString &ip, const QString &dstOid,
                  const QString &oid, const QString &msg);

private:
    QSNMPOid  mModuleOid;
    QTimer *timer;
};

#endif // AGENT_TRAP_H
