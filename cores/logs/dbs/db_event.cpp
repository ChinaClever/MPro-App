/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "db_event.h"


Db_Event::Db_Event()
{
    mHeadList << "event_type"<< "event_content";
}

Db_Event *Db_Event::bulid()
{
    static Db_Event* sington = nullptr;
    if(!sington) {
        sington = new Db_Event();
        sington->createTable();
    }
    return sington;
}

QJsonArray Db_Event::itemJson(const sEventItem &it)
{
    QJsonArray  var;
    var << it.datetime << it.event_type;
    var << it.event_content;
    return var;
}

bool Db_Event::modifyItem(const sEventItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);
    query.bindValue(":dtime",item.datetime);
    query.bindValue(":event_type",item.event_type);
    query.bindValue(":event_content",item.event_content);
    return sqlQuery(query);
}

void Db_Event::selectItem(QSqlQuery &query,sEventItem &item)
{
    item.id = query.value("id").toInt();
    item.datetime = query.value("dtime").toString();
    item.event_type = query.value("event_type").toString();
    item.event_content = query.value("event_content").toString();
}
