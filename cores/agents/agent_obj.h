#ifndef AGENT_OBJ_H
#define AGENT_OBJ_H
#include "commons.h"
#include "snmp/snmpagent.h"

class Agent_Obj : public QObject
{
    Q_OBJECT
public:
    explicit Agent_Obj(QObject *parent = nullptr);

private slots:
    void initAgentSlot();

protected:
    bool addOid(uchar addr, uint oid, const QString &oidPrefix,
                const QString &name, char *ptr, bool isWrite=true);
    bool addOidValue(uchar addr, uint oid, const QString &oidPrefix,
                     const QString &name, uint &value, bool isWrite=true);
protected:
    SnmpModule *mSnmp;
};

#endif // AGENT_OBJ_H
