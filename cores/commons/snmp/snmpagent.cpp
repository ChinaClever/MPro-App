#include "snmpagent.h"

SnmpModule *SnmpAgent::mModule = nullptr;
SnmpAgent::SnmpAgent(QObject *parent) : QObject{parent}
{
    snmpAgent = new QSNMPAgent("SNMP sub-agent");
    mModule = new SnmpModule(snmpAgent, parent);
    snmpAgent-> connect (snmpAgent, &QSNMPAgent::newLog, [=](QSNMPLogType_e logType, const QString & msg)
    {
        Q_UNUSED(logType)
        //qDebug() << msg;
    });
}

SnmpModule *SnmpAgent::bulid(QObject *parent)
{
    static SnmpAgent* sington = nullptr;
    if(!sington) sington = new SnmpAgent(parent);
    return mModule;
}
