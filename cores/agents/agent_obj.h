#ifndef AGENT_OBJ_H
#define AGENT_OBJ_H
#include "commons.h"
#include "snmp/snmpagent.h"
#define SNMP_TRAP_SIZE 4
struct sAgentCfg {
    int enV2;
    int enV3;
    QString usr;
    QString pwd;
    QString key;
    QString trap[SNMP_TRAP_SIZE];
};

class Agent_Obj : public QObject
{
    Q_OBJECT
public:
    explicit Agent_Obj(QObject *parent = nullptr);
    static sAgentCfg snmpCfg;
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
