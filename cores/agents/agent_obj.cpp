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
sAgentCfg Agent_Obj::snmpCfg;
Agent_Obj::Agent_Obj(QObject *parent)
    : QObject{parent}
{
    mMib = Agent_Mib::bulid();
    QTimer::singleShot(1355,this,SLOT(initAgentSlot()));
}

void Agent_Obj::initAgentSlot()
{
    mSnmp = SnmpAgent::bulid(this);
}

bool Agent_Obj::addOid(uchar addr, uint oid, const QString &oidPrefix, const QString &name, char *ptr, bool isWrite)
{
    sOidIt it;
    it.str = ptr;
    it.name = name;
    it.fieldId = addr;
    it.oid = oidPrefix;
    it.enWrited = isWrite;
    mMib->appendNode(it, oid);
    it.oid += QString::number(oid);
    return mSnmp->addOid(it);
}

bool Agent_Obj::addOidValue(uchar addr, uint oid, const QString &oidPrefix, const QString &name, uint &value, bool isWrite)
{
    sOidIt it;
    it.name = name;
    it.fieldId = addr;
    it.intPtr = &value;
    it.oid = oidPrefix;
    it.enWrited = isWrite;
    mMib->appendNode(it, oid);
    it.oid += QString::number(oid);
    return mSnmp->addOid(it);
}

