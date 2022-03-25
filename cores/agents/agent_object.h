#ifndef AGENT_OBJECT_H
#define AGENT_OBJECT_H
#include "commons.h"
#include "snmp/snmpagent.h"

class Agent_Object : public QObject
{
    Q_OBJECT
public:
    explicit Agent_Object(QObject *parent = nullptr);

signals:

protected:

    bool addOid(uchar addr, uint oid, const QString &oidPrefix, const QString &name, char *ptr);
    bool addOidValue(uchar addr, uint oid, const QString &oidPrefix,
                     const QString &name, uint &value);


protected:
    SnmpModule *mSnmp;
};

#endif // AGENT_OBJECT_H
