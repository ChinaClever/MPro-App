#include "snmpmodule.h"

SnmpModule::SnmpModule(QSNMPAgent *snmpAgent, QObject *parent)
    : QObject{parent}, QSNMPModule(snmpAgent)
{
    mSnmpAgent = snmpAgent;
    mSnmpAgent->setTrapsEnabled(true);
    mModuleOid = QSNMPOid() << 1 << 3 << 6 << 1 << 4 << 1 /* iso.org.dod.internet.private.enterprises */
                            << 30966 /* .example */
                            << 11; /* .myModule */
}

bool SnmpModule::addOid(const sOidIt &it)
{
    void *ptr = nullptr;  QSNMPType_e type;
    bool ret = !mShash.contains(it.name);
    if(!ret){ return ret; } if(it.intPtr){
        type = QSNMPType_Integer; ptr = (void *)it.intPtr;
    } else if(it.str){
        type = QSNMPType_OctetStr;  ptr = (void *)it.str;
    } else { ret = false; qDebug() << "Error: add oid";}

    QSNMPMaxAccess_e maxAccess = QSNMPMaxAccess_ReadOnly;
    if(it.enWrited)  maxAccess = QSNMPMaxAccess_ReadWrite;
    if(ptr) {mShash.insert(it.name, ptr);
        QSNMPVar *var = this->snmpCreateVar(it.name, type, maxAccess, mModuleOid, it.fieldId, toOid(it.oid));
        qDebug() << var->oidString() << "\t\t"<< var->name();
    }

    return ret;
}

QSNMPOid SnmpModule::toOid(const QString &oid)
{
    QSNMPOid res;
    for(auto &i : oid.split("."))  res << i.toInt();
    return res;
}


QVariant SnmpModule::snmpGetValue(const QSNMPVar * var)
{
    QVariant res;
    void *v = mShash.value(var->name());
    switch ((QSNMPType_e)var->type()) {
    case QSNMPType_Integer: res = *((int *)v); break;
    case QSNMPType_OctetStr: res = (char *)v; break;
    default: qDebug() << "Error: snmpGetValue" << var->fullName(); break;
    }

    return res;
}

bool SnmpModule::snmpSetValue(const QSNMPVar * var, const QVariant & v)
{
    void *ptr = mShash.value(var->name());
    switch ((QSNMPType_e)var->type()) {
    case QSNMPType_Integer: *((int *)ptr) = v.toUInt(); break;
    case QSNMPType_OctetStr: qstrcpy((char *)ptr, v.toByteArray().data()); break;
    default: qDebug() << "Error: snmpSetValue" << var->fullName(); break;
    } emit snmpSetSig(var->fieldId(), var->oidString(), v);

    return true;

    //    void *ptr = mShash.value(var->name());
    //    bool (*callback)(uint, const QString &, const QVariant &);
    //    callback = (bool (*)(uint, const QString &, const QVariant &))ptr;
    //    return callback(var->fieldId(), var->oidString(), v);
}

void SnmpModule::sendTrap(const QString & name, const QString &msg)
{
    QSNMPVar *var = this->snmpVar(name);
    mSnmpAgent->sendTrap(msg, mModuleOid, var->fieldId(), var);
}

void SnmpModule::sendTrap(const QString & name, quint32 fieldId, const QString &msg)
{
    QSNMPVar * var = this->snmpCreateVar(name, QSNMPType_OctetStr, QSNMPMaxAccess_Notify, mModuleOid, fieldId);
    mSnmpAgent->sendTrap(msg, mModuleOid, fieldId, var);
}
