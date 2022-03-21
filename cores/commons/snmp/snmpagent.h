#ifndef SNMPAGENT_H
#define SNMPAGENT_H

#include "agent/snmpmodule.h"

class SnmpAgent : public QObject
{
    Q_OBJECT
    explicit SnmpAgent(QObject *parent = nullptr);
public:
    static SnmpModule *bulid(QObject *parent = nullptr);

private:
    static SnmpModule *mModule;
    QSNMPAgent *snmpAgent;
};

#endif // SNMPAGENT_H
