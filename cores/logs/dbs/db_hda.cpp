/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "db_hda.h"

Db_Hda::Db_Hda()
{
    mHeadList << "addr" << "type" << "topic" << "indexes" << "value";
}

Db_Hda *Db_Hda::bulid()
{
    static Db_Hda* sington = nullptr;
    if(!sington) {
        sington = new Db_Hda();
        sington->createTable();
    }
    return sington;
}

QJsonArray Db_Hda::itemJson(const sHdaItem &it)
{
    QJsonArray  var;
    var << it.datetime << it.addr << it.type;
    var << it.topic << it.indexes << it.value;
    return var;
}

bool Db_Hda::modifyItem(const sHdaItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);
    query.bindValue(":addr",item.addr);
    query.bindValue(":type",item.type);
    query.bindValue(":topic",item.topic);
    query.bindValue(":dtime",item.datetime);
    query.bindValue(":indexes",item.indexes);
    query.bindValue(":value",item.value);
    return sqlQuery(query);
}

void Db_Hda::selectItem(QSqlQuery &query, sHdaItem &item)
{
    item.id = query.value("id").toInt();
    item.addr = query.value("addr").toString();
    item.type = query.value("type").toString();
    item.topic = query.value("topic").toString();
    item.datetime = query.value("dtime").toString();
    item.indexes = query.value("indexes").toString();
    item.value = query.value("value").toString();
}
