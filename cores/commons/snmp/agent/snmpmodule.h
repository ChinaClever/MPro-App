#ifndef SNMPMODULE_H
#define SNMPMODULE_H

#include <QtCore>
#include "QSNMP.h"

struct sOidIt{
    sOidIt() : fieldId(0), intPtr(nullptr), str(nullptr), callback(nullptr){ }
    QString name;
    quint32 fieldId;
    QString oid;
    int *intPtr;
    char *str;
    bool (*callback)(uint, const QString &, const QVariant &);
};

class SnmpModule : public QObject, public QSNMPModule
{
    Q_OBJECT
public:
    explicit SnmpModule(QSNMPAgent * snmpAgent, QObject *parent = nullptr);

    bool addOid(const sOidIt &it);
    virtual QVariant snmpGetValue(const QSNMPVar * var);
    virtual bool snmpSetValue(const QSNMPVar * var, const QVariant & v);

    void sendTrap(const QString & name, const QString &msg);
    void sendTrap(const QString & name, quint32 fieldId, const QString &msg);

protected:
    QSNMPOid toOid(const QString &oid);

private:
    QSNMPOid  mModuleOid;
    QSNMPAgent *mSnmpAgent;
    QHash<QString, void *> mShash;
    QHash<QString, QVariant> mRhash;
};

#endif // SNMPMODULE_H