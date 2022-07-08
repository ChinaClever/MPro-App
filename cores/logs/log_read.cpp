/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "log_read.h"

Log_Read::Log_Read(QObject *parent)
    : QObject{parent}
{

}

template <typename T, typename U>
static QString log_read_page(U *db, int page, int cnt)
{
    QVector<T> its;
    int minId = db->minId();
    if(page) {
        int min = (page-1) * cnt;
        int max = page * cnt;
        if(minId > 1) {minId--; min += minId; max += minId;}
        its = db->selectBetween(min, max);
    } else its = db->selectAll();
    return db->toPageJson(its, minId);
}

template <typename T, typename U>
static QString log_read_once(U *db, int id)
{
    int minId = db->minId();
    T it = db->selectById(id + minId);
    return db->toOnceJson(it, minId);
}

QString Log_Read::log_readOnce(int type, int id)
{
    QVariant v; switch (type) {
    case eLogs::eUserLog: v = log_read_once<sUserItem>(Db_User::bulid(), id); break;
    case eLogs::eAlarmLog: v = log_read_once<sAlarmItem>(Db_Alarm::bulid(), id); break;
    case eLogs::eOpLog: v = log_read_once<sOpItem>(Db_Op::bulid(), id);  break;
    case eLogs::eSysLog: v = log_read_once<sSysItem>(Db_Sys::bulid(), id);  break;
    case eLogs::eEleLog: v = log_read_once<sEleItem>(Db_Ele::bulid(), id);  break;
    default: qDebug() << Q_FUNC_INFO << type; break;
    }

    return v.toString();
}


QString Log_Read::log_readPage(int type, int page, int cnt)
{
    QVariant v; switch (type) {
    case eLogs::eUserLog: v = log_read_page<sUserItem>(Db_User::bulid(), page, cnt); break;
    case eLogs::eAlarmLog: v = log_read_page<sAlarmItem>(Db_Alarm::bulid(), page, cnt); break;
    case eLogs::eOpLog: v = log_read_page<sOpItem>(Db_Op::bulid(), page, cnt);  break;
    case eLogs::eSysLog: v = log_read_page<sSysItem>(Db_Sys::bulid(), page, cnt);  break;
    case eLogs::eEleLog: v = log_read_page<sEleItem>(Db_Ele::bulid(), page, cnt);  break;
    default: qDebug() << Q_FUNC_INFO << type; break;
    }

    return v.toString();
}

Sql_Statement *Log_Read::getSql(int type)
{
    Sql_Statement *sql = nullptr;
    switch (type) {
    case eLogs::eUserLog: sql = Db_User::bulid(); break;
    case eLogs::eAlarmLog: sql = Db_Alarm::bulid();  break;
    case eLogs::eOpLog: sql = Db_Op::bulid();  break;
    case eLogs::eSysLog: sql = Db_Sys::bulid();  break;
    case eLogs::eEleLog: sql = Db_Ele::bulid();  break;
    default: qDebug() << Q_FUNC_INFO << type; break;
    }
    return sql;
}


QString Log_Read::log_readFun(const sLogFcIt &it)
{
    QString res;
    Sql_Statement *sql = getSql(it.type);
    switch (it.fc) {
    case eLogFc::eLog_clear: sql->clear(); break;
    case eLogFc::eLog_cnt: res = sql->counts(); break;
    case eLogFc::eLog_read: res = log_readOnce(it.type, it.id); break;
    case eLogFc::eLog_page: res = log_readPage(it.type, it.id, it.noe); break;
    default: qDebug() << Q_FUNC_INFO << it.fc; break;
    }
    return res;
}
