#include "ipc_logserver.h"

IPC_LogServer::IPC_LogServer(QObject *parent)
    : IPC_ObjServer{parent}
{

}

IPC_LogServer *IPC_LogServer::bulid(QObject *parent)
{
    static IPC_LogServer *sington = nullptr;
    if(!sington) {
        sington = new IPC_LogServer(parent);
        sington->initFunction(IPC_KEY_LOG);
    }
    return sington;
}

QList<const char *> IPC_LogServer::busRecvMethods()
{
    QList<const char *> res;
    res << SLOT(dbus_recv_slot(int, QStringList));
    return res;
}

void IPC_LogServer::userWrite(const QStringList &ls)
{

}

void IPC_LogServer::dbus_recv_slot(int id, const QStringList &ls)
{
    switch (id) {
    case eUserLog: userWrite(ls); break;
    }

    qDebug()  << "AAAAAAAA" << id << ls;
}

QStringList IPC_LogServer::dbus_reply_slot(int id, int page)
{
    QVariant v = "aaaa";//mCfg->readCfg(page, value, id);
    QStringList res{v.toString()};
    res.append("222");
    return res;
}
