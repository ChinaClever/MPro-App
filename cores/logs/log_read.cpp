/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "log_read.h"
#include "commons.h"

Log_Read::Log_Read(QObject *parent)
    : Log_Sys{parent}
{

}

template <typename T, typename U>
static QString log_read_page(U *db, int id, int cnt)
{
    QVector<T> its; //if(id)id--;
    int minId = db->minId();
    if(minId >= 0) {
        int min = id + minId;
        int max = min + cnt - 1;
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
    case eLogs::eOtaLog: v = log_read_once<sOtaItem>(Db_Ota::bulid(), id); break;
    case eLogs::eHdaLog: v = log_read_once<sHdaItem>(Db_Hda::bulid(), id); break;
    case eLogs::eEventLog: v = log_read_once<sEventItem>(Db_Event::bulid(), id); break;
    case eLogs::eAlarmLog: v = log_read_once<sAlarmItem>(Db_Alarm::bulid(), id); break;
    default: qDebug() << Q_FUNC_INFO << type; break;
    }

    return v.toString();
}

QString Log_Read::log_readPage(int type, int id, int cnt)
{
    QVariant v; switch (type) {
    case eLogs::eOtaLog: v = log_read_page<sOtaItem>(Db_Ota::bulid(), id, cnt); break;
    case eLogs::eHdaLog: v = log_read_page<sHdaItem>(Db_Hda::bulid(), id, cnt); break;
    case eLogs::eEventLog: v = log_read_page<sEventItem>(Db_Event::bulid(), id, cnt); break;
    case eLogs::eAlarmLog: v = log_read_page<sAlarmItem>(Db_Alarm::bulid(), id, cnt); break;
    default: qDebug() << Q_FUNC_INFO << type; break;
    }

    return v.toString();
}

Sql_Statement *Log_Read::getSql(int type)
{
    Sql_Statement *sql = nullptr; switch (type) {
    case eLogs::eOtaLog: sql = Db_Ota::bulid(); break;
    case eLogs::eHdaLog: sql = Db_Hda::bulid(); break;
    case eLogs::eEventLog: sql = Db_Event::bulid(); break;
    case eLogs::eAlarmLog: sql = Db_Alarm::bulid(); break;
    default: qDebug() << Q_FUNC_INFO << type; break;
    }
    return sql;
}


QString Log_Read::log_readFun(const sLogFcIt &it)
{    
    //cout << it.type << it.fc << it.id;
    Sql_Statement *sql = getSql(it.type); if(!sql) return "";
    QString res; QWriteLocker locker(mRwLock); switch (it.fc) {
    case eLogFc::eLog_clear: sql->clear(); break;
    case eLogFc::eLog_cnt: res = QString::number(sql->counts()); break;
    case eLogFc::eLog_readOnce: res = log_readOnce(it.type, it.id); break;
    case eLogFc::eLog_read: res = log_readPage(it.type, it.id, it.cnt); break;
    default: qDebug() << Q_FUNC_INFO << it.fc; break;
    } //cout << it.type << it.fc << it.id << res;

    return res;
}

QString Log_Read::log_readHda(const sLogHdaIt &it)
{
    QString cmd = "where ";
    if(it.start.size()) {
        QString endDateStr = it.end;
        if(it.end.isEmpty()) endDateStr = QDate::currentDate().toString("yyyy-MM-dd");
        cmd += QString("dtime between '%1 00:00:00' and '%2 23:59:59' ").arg(it.start, endDateStr);
    }

    cmd += QString(" and addr = '%1'").arg(it.addr);
    if(it.type) cmd += QString(" and type = '%1'").arg(it.type);
    if(it.topic) cmd += QString(" and topic = '%1'").arg(it.topic);
    if(it.index) cmd += QString(" and indexes = '%1'").arg(it.index);

    QWriteLocker locker(mRwLock);
    QString res; Db_Hda *db = Db_Hda::bulid();
    QVector<sHdaItem> its = db->selectItems(cmd);
    if(its.size()) {
        for(int i=0; i<its.size(); ++i) its[i].id = i;
        int minId = its.first().id;
        res = db->toPageJson(its, minId);
    } else qDebug() << Q_FUNC_INFO << cmd;
    cout << its.size() << cmd  << res;

    return res;
}

QString Log_Read::logFun(const sCfgItem &cfg)
{
    sLogFcIt it; it.type = cfg.fc; it.fc = cfg.addr;
    it.id = cfg.id; it.cnt = 20;
    return log_readFun(it);
}
