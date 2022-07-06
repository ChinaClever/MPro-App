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

template <typename T, typename U>
static QString ipc_log_read(U *db, int page, int cnt)
{
    QVector<T> its;
    if(page) {
        int min = (page-1) * cnt;
        int max = page * cnt;
        int minId = db->minId();
        if(minId > 1) {minId--; min += minId; max += minId;}
        its = db->selectBetween(min, max);
    } else its = db->selectAll();
    return db->toJson(its);
}

QString IPC_LogServer::readLog(int id, int page, int cnt)
{
    QVariant v; switch (id) {
    case eUserLog: v = ipc_log_read<sUserItem>(Db_User::bulid(), page, cnt); break;
    case eAlarmLog: v = ipc_log_read<sAlarmItem>(Db_Alarm::bulid(), page, cnt); break;
    case eOpLog: v = ipc_log_read<sOpItem>(Db_Op::bulid(), page, cnt);  break;
    case eSysLog: v = ipc_log_read<sSysItem>(Db_Sys::bulid(), page, cnt);  break;
    case eEleLog: v = ipc_log_read<sEleItem>(Db_Ele::bulid(), page, cnt);  break;
    default: qDebug() << Q_FUNC_INFO << id; break;
    }

    return v.toString();
}

Sql_Statement *IPC_LogServer::getSql(int id)
{
    Sql_Statement *sql = nullptr;
    switch (id) {
    case eUserLog: sql = Db_User::bulid(); break;
    case eAlarmLog: sql = Db_Alarm::bulid();  break;
    case eOpLog: sql = Db_Op::bulid();  break;
    case eSysLog: sql = Db_Sys::bulid();  break;
    case eEleLog: sql = Db_Ele::bulid();  break;
    default: qDebug() << Q_FUNC_INFO << id; break;
    }
    return sql;
}


QString IPC_LogServer::logFun(const sIpcLog &it)
{
    QString res;
    Sql_Statement *sql = getSql(it.id);
    switch (it.fc) {
    case 3: sql->clear(); break;
    case 1: res = sql->counts(); break;
    case 2: res = readLog(it.id, it.page, it.noe); break;
    default: qDebug() << Q_FUNC_INFO << it.fc; break;
    }
    return res;
}

