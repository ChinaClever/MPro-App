/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "db_hardware.h"

Db_Hardware::Db_Hardware()
{
    mHeadList << "module"<< "content";
}

Db_Hardware *Db_Hardware::bulid()
{
    static Db_Hardware* sington = nullptr;
    if(!sington) {
        sington = new Db_Hardware();
        sington->createTable();
    }
    return sington;
}

QJsonArray Db_Hardware::itemJson(const sHardwareItem &it)
{
    QJsonArray  var;
    var << it.datetime << it.module;
    var << it.content;
    return var;
}

bool Db_Hardware::modifyItem(const sHardwareItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);
    query.bindValue(":dtime",item.datetime);
    query.bindValue(":module",item.module);
    query.bindValue(":content",item.content);
    return sqlQuery(query);
}

void Db_Hardware::selectItem(QSqlQuery &query,sHardwareItem &item)
{
    item.id = query.value("id").toInt();
    item.datetime = query.value("dtime").toString();
    item.module = query.value("module").toString();
    item.content = query.value("content").toString();
}
