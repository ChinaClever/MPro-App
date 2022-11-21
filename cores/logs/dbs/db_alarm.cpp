/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "db_alarm.h"


Db_Alarm::Db_Alarm()
{
    mHeadList << "alarm_status" << "alarm_content";
}

Db_Alarm *Db_Alarm::bulid()
{
    static Db_Alarm* sington = nullptr;
    if(!sington) {
        sington = new Db_Alarm();
        sington->createTable();
    }
    return sington;
}

QJsonArray Db_Alarm::itemJson(const sAlarmItem &it)
{
    QJsonArray  var;
    var << it.datetime << it.alarm_status << it.alarm_content;
    return var;
}

bool Db_Alarm::modifyItem(const sAlarmItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);
    query.bindValue(":dtime",item.datetime);
    query.bindValue(":alarm_status",item.alarm_status);
    query.bindValue(":alarm_content",item.alarm_content);
    return sqlQuery(query);
}

void Db_Alarm::selectItem(QSqlQuery &query,sAlarmItem &item)
{
    item.id = query.value("id").toInt();
    item.datetime = query.value("dtime").toString();
    item.alarm_status = query.value("alarm_status").toString();
    item.alarm_content = query.value("alarm_content").toString();
}
