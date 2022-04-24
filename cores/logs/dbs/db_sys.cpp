/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "db_sys.h"

Db_Sys::Db_Sys()
{
    mHeadList << "module"<< "content";
}

Db_Sys *Db_Sys::bulid()
{
    static Db_Sys* sington = nullptr;
    if(!sington) {
        sington = new Db_Sys();
        sington->createTable();
    }
    return sington;
}

QJsonArray Db_Sys::itemJson(const sSysItem &it)
{
    QJsonArray  var;
    var << it.datetime << it.module;
    var << it.content;
    return var;
}

bool Db_Sys::modifyItem(const sSysItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);
    query.bindValue(":dtime",item.datetime);
    query.bindValue(":module",item.module);
    query.bindValue(":content",item.content);
    return sqlQuery(query);
}

void Db_Sys::selectItem(QSqlQuery &query,sSysItem &item)
{
    item.id = query.value("id").toInt();
    item.datetime = query.value("dtime").toString();
    item.module = query.value("module").toString();
    item.content = query.value("content").toString();
}

