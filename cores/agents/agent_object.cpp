#include "agent_object.h"

Agent_Object::Agent_Object(QObject *parent)
    : QObject{parent}
{
//    mSnmp = SnmpAgent::bulid(this);
}


bool Agent_Object::addOid(uchar addr, uint oid, const QString &oidPrefix, const QString &name, char *ptr)
{
    sOidIt it;
    it.str = ptr;
    it.name = name;
    it.fieldId = addr;
    it.oid = oidPrefix + QString::number(oid);
    qDebug() << it.oid  << "\t\t" << it.name ;
    return true;
//    return mSnmp->addOid(it);
}

bool Agent_Object::addOidValue(uchar addr, uint oid, const QString &oidPrefix, const QString &name, uint &value)
{
    sOidIt it;
    it.name = name;
    it.fieldId = addr;
    it.intPtr = &value;
    it.oid = oidPrefix + QString::number(oid);
     qDebug() << it.oid  << "\t\t" << it.name ;
    return true;
//    return mSnmp->addOid(it);
}
