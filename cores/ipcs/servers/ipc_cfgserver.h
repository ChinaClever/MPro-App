#ifndef IPC_CFGSERVER_H
#define IPC_CFGSERVER_H

#include "ipc_webserver.h"
#include "cfg_obj.h"

class IPC_CfgServer : public IPC_ObjServer
{
    Q_OBJECT
    explicit IPC_CfgServer(QObject *parent = nullptr);
public:
    static IPC_CfgServer *bulid(QObject *parent = nullptr);

public slots:
    void dbus_recv_slot(const QString &prefix, const QString &key, const QString &value);
    QStringList dbus_reply_slot(const QString &prefix, const QString &key, const QString &value);

protected:
    QList<const char *> busRecvMethods();
    Cfg_Obj *mCfg;
};

#endif // IPC_CFGSERVER_H
