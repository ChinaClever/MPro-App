/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "agent_object.h"

Agent_Object::Agent_Object(QObject *parent)
    : QObject{parent}
{
    mSnmp = SnmpAgent::bulid(this);
    connect(mSnmp, &SnmpModule::snmpSetSig, this, &Agent_Object::snmpSetSlot);
}


bool Agent_Object::addOid(uchar addr, uint oid, const QString &oidPrefix, const QString &name, char *ptr, bool isWrite)
{
    sOidIt it;
    it.str = ptr;
    it.name = name;
    it.fieldId = addr;
    it.enWrited = isWrite;
    it.oid = oidPrefix + QString::number(oid);
    return mSnmp->addOid(it);
}

bool Agent_Object::addOidValue(uchar addr, uint oid, const QString &oidPrefix, const QString &name, uint &value, bool isWrite)
{
    sOidIt it;
    it.name = name;
    it.fieldId = addr;
    it.intPtr = &value;
    it.enWrited = isWrite;
    it.oid = oidPrefix + QString::number(oid);
    return mSnmp->addOid(it);
}

void Agent_Object::snmpSetSlot(uint addr, const QString &oid, const QVariant &value)
{

}
