/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "db_ota.h"


Db_Ota::Db_Ota()
{
    mHeadList << "module"<< "content";
}

Db_Ota *Db_Ota::bulid()
{
    static Db_Ota* sington = nullptr;
    if(!sington) {
        sington = new Db_Ota();
        sington->createTable();
    }
    return sington;
}

QJsonArray Db_Ota::itemJson(const sOtaItem &it)
{
    QJsonArray  var;
    var << it.datetime << it.module;
    var << it.content;
    return var;
}

bool Db_Ota::modifyItem(const sOtaItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);
    query.bindValue(":dtime",item.datetime);
    query.bindValue(":module",item.module);
    query.bindValue(":content",item.content);
    return sqlQuery(query);
}

void Db_Ota::selectItem(QSqlQuery &query,sOtaItem &item)
{
    item.id = query.value("id").toInt();
    item.datetime = query.value("dtime").toString();
    item.module = query.value("module").toString();
    item.content = query.value("content").toString();
}
