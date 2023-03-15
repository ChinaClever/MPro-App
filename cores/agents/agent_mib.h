#ifndef AGENT_MIB_H
#define AGENT_MIB_H
#include "commons.h"
#include "snmp/snmpagent.h"
class Agent_Mib
{
    Agent_Mib();
public:
    static Agent_Mib *bulid();
    void initModule(){m_module = "mpro";}
    void appendNode(const sOidIt &it, uint oid);
    void save();

private:
    void createHead();
    void appendModule(const sOidIt &it);
    void appendModule(const QString &name, int oid);

private:
    QString m_mib;
    QString m_module;
};

#endif // AGENT_MIB_H
