#include "ipc_logserver.h"
#include "log_core.h"

IPC_LogServer::IPC_LogServer(QObject *parent)
    : IPC_ObjServer{parent}
{

}

IPC_LogServer *IPC_LogServer::bulid(QObject *parent)
{
    static IPC_LogServer *sington = nullptr;
    if(!sington) {
        sington = new IPC_LogServer(parent);
        sington->initFunction(IPC_KEY_LOG, true);
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
    int k=0;
    sUserItem it;

    it.name = ls.at(k++);
    it.pwd = ls.at(k++);
    it.jurisdiction = ls.at(k++);

    it.email = ls.at(k++);
    it.telephone = ls.at(k++);
    it.remarks = ls.at(k++);
    Log_Core::bulid(this)->append(it);
}


void IPC_LogServer::opWrite(const QStringList &ls)
{
    int k=0;
    sOpItem it;

    it.op_src = ls.at(k++);
    it.content = ls.at(k++);
    Log_Core::bulid(this)->append(it);
    qDebug() << "AAAAAAA" << ls;
}

void IPC_LogServer::dbus_recv_slot(int id, const QStringList &ls)
{
    switch (id) {
    case eUserLog: userWrite(ls); break;
    case eOpLog: opWrite(ls); break;
    }
}

QString IPC_LogServer::userRead(int page)
{
    QVector<sUserItem> its;
    Db_User *db = Db_User::bulid();
    if(page) {
        int min = (page-1) * 30;
        int max = page *30;
        its = db->selectBetween(min, max);
    } else its = db->selectAll();
    return db->toJson(its);
}


QString IPC_LogServer::opRead(int page)
{
    QVector<sOpItem> its;
    Db_Op *db = Db_Op::bulid();
    if(page) {
        int min = (page-1) * 30;
        int max = page *30;
        its = db->selectBetween(min, max);
        qDebug() << "BBBBBBBB" << its.size() << page;
    } else its = db->selectAll();
    return db->toJson(its);
}

QStringList IPC_LogServer::dbus_reply_slot(int id, int page)
{
    QVariant v; switch (id) {
    case eUserLog: v = userRead(page); break;
    case eOpLog: v = opRead(page);  break;
    }

    QStringList res{v.toString()};
    return res;
}


QByteArray IPC_LogServer::lsRecv(const QByteArray &v)
{
    qDebug() << "lsocket_recv:" << v;
    return v;
}
