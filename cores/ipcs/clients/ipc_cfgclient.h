#ifndef IPC_CFGCLIENT_H
#define IPC_CFGCLIENT_H

#include "ipc_logclient.h"

class IPC_CfgClient : public IPC_ObjClient
{
    Q_OBJECT
    explicit IPC_CfgClient(QObject *parent = nullptr);
public:
    static IPC_CfgClient *bulid(QObject *parent = nullptr);
    bool setValue(const QString &prefix, const QString &key, const QVariant &value);
    QVariant value(const QString &prefix, const QString &key, const QVariant &defaultValue = QVariant());

private:
    bool inputCheck(const QVariantList &values);
};

#endif // IPC_CFGCLIENT_H
