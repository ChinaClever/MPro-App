/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "agent_obj.h"

Agent_Obj::Agent_Obj(QObject *parent)
    : QObject{parent}
{
    //initSnmpConf();
    mSnmp = SnmpAgent::bulid(this);

    //connect(mSnmp, &SnmpModule::snmpSetSig, this, &Agent_Obj::snmpSetSlot);
}

void Agent_Obj::initSnmpConf()
{
    QString cmd = "echo \"123456\" | sudo -S service snmpd stop";
    system(cmd.toLatin1().data());

    QString fn = "/home/lzy/work/NPDU/cores/commons/snmp/agent/net-snmp/";
    cmd = "echo \"123456\" | sudo -S snmpd -f -Lo -C -c ";
    cmd += fn +"snmpd.conf &";
    system(cmd.toLatin1().data());
}

bool Agent_Obj::addOid(uchar addr, uint oid, const QString &oidPrefix, const QString &name, char *ptr, bool isWrite)
{
    sOidIt it;
    it.str = ptr;
    it.name = name;
    it.fieldId = addr;
    it.enWrited = isWrite;
    it.oid = oidPrefix + QString::number(oid);
    return mSnmp->addOid(it);
}

bool Agent_Obj::addOidValue(uchar addr, uint oid, const QString &oidPrefix, const QString &name, uint &value, bool isWrite)
{
    sOidIt it;
    it.name = name;
    it.fieldId = addr;
    it.intPtr = &value;
    it.enWrited = isWrite;
    it.oid = oidPrefix + QString::number(oid);
    return mSnmp->addOid(it);
}

void Agent_Obj::snmpSetSlot(uint addr, const QString &oid, const QVariant &value)
{

}
