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
        //qDebug() << var->oidString() << "\t" /*<<  it.enWrited*/ << "\t"<< var->name();
    }

    return ret;
}

void SnmpModule::addSysOid()
{
    QSNMPType_e type = QSNMPType_ObjectId;
    QSNMPMaxAccess_e maxAccess = QSNMPMaxAccess_ReadOnly;
    QSNMPOid  moduleOid = QSNMPOid() << 1 << 3 << 6 << 1 << 2 << 1;
    this->snmpCreateVar("sysObjectID", type, maxAccess, moduleOid, 1, toOid("2"));
    type = QSNMPType_Integer; maxAccess = QSNMPMaxAccess_ReadOnly;
    this->snmpCreateVar("sysService", type, maxAccess, moduleOid, 1, toOid("7"));
    //    type = QSNMPType_OctetStr; maxAccess = QSNMPMaxAccess_ReadWrite;
    //    this->snmpCreateVar("sysLocation", type, maxAccess, moduleOid, 1, toOid("6"));
}

QSNMPOid SnmpModule::toOid(const QString &oid)
{
    QSNMPOid res;
    for(auto &i : oid.split("."))  res << i.toInt();
    return res << 0; ////////===========
}

QVariant SnmpModule::snmpGetValue(const QSNMPVar * var)
{    
    void *v = mShash.value(var->name());
    QVariant res; /*QString str;*/ if(v) {
        switch ((QSNMPType_e)var->type()) {
        case QSNMPType_Integer: res = *((int *)v); break;
        case QSNMPType_OctetStr: res = (char *)v; break;
        default: qDebug() << "Error: snmpGetValue" << var->fullName(); break;
        }
    } else {
        if(var->name().contains("sysObjectID")) {
            QSNMPOid  oid = QSNMPOid() << 1 << 3 << 6 << 1 << 4 << 1 << 30966 << 11;
            res.setValue(oid); //res = "1.3.6.1.4.1.30966.11";
        } else if(var->name().contains("sysLocation")) res = "Location";
        else if(var->name().contains("sysService")) res = 4;
        else qDebug() << var->name();
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
    } emit snmpSetSig(var->fieldId(), var->oid(), v);

    return true;

    //    void *ptr = mShash.value(var->name());
    //    bool (*callback)(uint, const QString &, const QVariant &);
    //    callback = (bool (*)(uint, const QString &, const QVariant &))ptr;
    //    return callback(var->fieldId(), var->oidString(), v);
}

bool SnmpModule::sendTrap(const QSNMPOid & oid, const QString &msg)
{    
    QSNMPVar *var = this->snmpVar(oid); bool ret = true;
    if(var) mSnmpAgent->sendTrap(msg, mModuleOid, var->fieldId(), var);
    else ret = false;
    return ret;
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
