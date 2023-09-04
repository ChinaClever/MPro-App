#ifndef SNMPMODULE_H
#define SNMPMODULE_H

#include <QtCore>
#include "QSNMP.h"

struct sOidIt{
    sOidIt() : fieldId(0), intPtr(nullptr), str(nullptr), enWrited(false){ }
    QString name;
    quint32 fieldId;
    QString oid;
    uint *intPtr;
    char *str;
    bool enWrited;
    //bool (*callback)(uint, const QString &, const QVariant &);
};

class SnmpModule : public QObject, public QSNMPModule
{
    Q_OBJECT
public:
    explicit SnmpModule(QSNMPAgent * snmpAgent, QObject *parent = nullptr);

    void addSysOid();
    bool addOid(const sOidIt &it);
    virtual QVariant snmpGetValue(const QSNMPVar * var);
    virtual bool snmpSetValue(const QSNMPVar * var, const QVariant & v);

    QSNMPOid moduleOid() {return mModuleOid;}
    bool sendTrap(const QSNMPOid & oid, const QString &msg="Trap");
    void sendTrap(const QString & name, const QString &msg);
    void sendTrap(const QString & name, quint32 fieldId, const QString &msg);

signals:
    void snmpSetSig(uint, const QSNMPOid &, const QVariant &);

protected:
    QSNMPOid toOid(const QString &oid);

private:
    QSNMPOid  mModuleOid;
    QSNMPAgent *mSnmpAgent;
    QHash<QString, void *> mShash;
//    QHash<QString, QVariant> mRhash;
};

#endif // SNMPMODULE_H
