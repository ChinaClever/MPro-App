#ifndef IPC_SETTINGCLIENT_H
#define IPC_SETTINGCLIENT_H

#include "ipc_logclient.h"

class IPC_SettingClient : public IPC_ObjClient
{
    Q_OBJECT
    explicit IPC_SettingClient(QObject *parent = nullptr);
public:
    static IPC_SettingClient *bulid(QObject *parent = nullptr);
    bool setValue(const QString &prefix, const QString &key, const QVariant &value);
    QVariant value(const QString &prefix, const QString &key, const QVariant &defaultValue = QVariant());

private:
    bool inputCheck(const QVariantList &values);
};

#endif // IPC_SETTINGCLIENT_H
