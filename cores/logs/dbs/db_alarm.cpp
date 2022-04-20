#include "db_alarm.h"


Db_Alarm::Db_Alarm()
{
    mHeadList << "module" << "state" << "content";
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
    var << it.datetime << it.module;
    var << it.state << it.content;
    return var;
}

bool Db_Alarm::modifyItem(const sAlarmItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);
    query.bindValue(":dtime",item.datetime);
    query.bindValue(":module",item.module);
    query.bindValue(":state",item.state);
    query.bindValue(":content",item.content);
    return sqlQuery(query);
}

void Db_Alarm::selectItem(QSqlQuery &query,sAlarmItem &item)
{
    item.id = query.value("id").toInt();
    item.datetime = query.value("dtime").toString();
    item.module = query.value("module").toString();
    item.state = query.value("state").toString();
    item.content = query.value("content").toString();
}
