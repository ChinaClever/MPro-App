/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "agent_obj.h"

Agent_Obj::Agent_Obj(QObject *parent)
    : QObject{parent}
{
//    QTimer::singleShot(5,this,SLOT(snmpStop()));
//    QTimer::singleShot(2050,this,SLOT(snmpConf()));
    QTimer::singleShot(550,this,SLOT(snmpStart()));
}

void Agent_Obj::snmpStop()
{
    QString cmd = "echo \"123456\" | sudo -S service snmpd stop";
    system(cmd.toLatin1().data());
}

void Agent_Obj::snmpConf()
{
    QString fn = "/home/lzy/work/NPDU/cores/commons/snmp/agent/net-snmp/";
    QString cmd = "echo \"123456\" | sudo -S snmpd -f -Lo -C -c ";
    cmd += fn +"snmpd.conf &";
    system(cmd.toLatin1().data());
}

void Agent_Obj::snmpStart()
{
    mSnmp = SnmpAgent::bulid(this);
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

