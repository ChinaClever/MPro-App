/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ipc_cfgserver.h"

IPC_CfgServer::IPC_CfgServer(QObject *parent)
    : IPC_ObjServer{parent}
{
    mCfg = Cfg_Obj::bulid(CFG_FN, this);
}

IPC_CfgServer *IPC_CfgServer::bulid(QObject *parent)
{
    static IPC_CfgServer *sington = nullptr;
    if(!sington) {
        sington = new IPC_CfgServer(parent);
        sington->initFunction(IPC_KEY_CFG);
    }
    return sington;
}

QList<const char *> IPC_CfgServer::busRecvMethods()
{
    QList<const char *> res;
    res << SLOT(dbus_recv_slot(QString, QString, QString));
    return res;
}

void IPC_CfgServer::dbus_recv_slot(const QString &prefix, const QString &key, const QString &value)
{
    mCfg->writeCfg(key, value, prefix);
}

QStringList IPC_CfgServer::dbus_reply_slot(const QString &prefix, const QString &key, const QString &value)
{
    QVariant v = mCfg->readCfg(key, value, prefix);
    QStringList res{v.toString()};
    return res;
}

