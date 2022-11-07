/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "db_event.h"


Db_Event::Db_Event()
{
    mHeadList << "type"<< "content";
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
    var << it.datetime << it.type;
    var << it.content;
    return var;
}

bool Db_Event::modifyItem(const sEventItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);
    query.bindValue(":dtime",item.datetime);
    query.bindValue(":type",item.type);
    query.bindValue(":content",item.content);
    return sqlQuery(query);
}

void Db_Event::selectItem(QSqlQuery &query,sEventItem &item)
{
    item.id = query.value("id").toInt();
    item.datetime = query.value("dtime").toString();
    item.type = query.value("type").toString();
    item.content = query.value("content").toString();
}
