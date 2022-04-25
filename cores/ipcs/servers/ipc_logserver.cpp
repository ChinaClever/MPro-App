/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
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
    int k=0; sOpItem it;
    it.op_src = ls.at(k++);
    it.content = ls.at(k++);
    Log_Core::bulid(this)->append(it);
}

void IPC_LogServer::dbus_recv_slot(int id, const QStringList &ls)
{
    switch (id) {
    case eUserLog: userWrite(ls); break;
    case eOpLog: opWrite(ls); break;
    default: qDebug() << Q_FUNC_INFO << id; break;
    }
}

template <typename T, typename U>
static QString ipc_log_read(U *db, int page)
{
    QVector<T> its;
    int cnt = 45; if(page) {
        int min = (page-1) * cnt;
        int max = page * cnt;
        its = db->selectBetween(min, max);
    } else its = db->selectAll();
    return db->toJson(its);
}

QStringList IPC_LogServer::dbus_reply_slot(int id, int page)
{
    QVariant v; switch (id) {
    case eUserLog: v = ipc_log_read<sUserItem>(Db_User::bulid(), page); break;
    case eOpLog: v = ipc_log_read<sOpItem>(Db_Op::bulid(), page);  break;
    case eSysLog: v = ipc_log_read<sSysItem>(Db_Sys::bulid(), page);  break;
    case eAlarmLog: v = ipc_log_read<sAlarmItem>(Db_Alarm::bulid(), page); break;
    default: qDebug() << Q_FUNC_INFO << id; break;
    }

    QStringList res{v.toString()};
    return res;
}


QByteArray IPC_LogServer::lsRecv(const QByteArray &v)
{
    QStringList ls = QString(v).split(";");
    int id = ls.first().toInt();
    int fc = ls.last().toInt();

    Sql_Statement *sql = nullptr;
    switch (id) {
    case eUserLog: sql = Db_User::bulid(); break;
    case eOpLog: sql = Db_Op::bulid();  break;
    case eSysLog: sql = Db_Sys::bulid();  break;
    default: qDebug() << Q_FUNC_INFO << id; break;
    }

    QByteArray res; if(sql) {
        switch (fc) {
        case 1: sql->clear(); break;
        case 2: id = sql->counts(); res = QString::number(id).toLocal8Bit(); break;
        }
    }

    return res;
}
