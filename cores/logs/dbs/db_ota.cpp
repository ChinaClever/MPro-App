/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "db_ota.h"


Db_Ota::Db_Ota()
{
    mHeadList << "ver"<< "md5" << "remark" << "oldVersion" << "releaseDate";
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
    var << it.datetime << it.ver << it.md5 << it.remark;
    var << it.oldVersion <<  it.releaseDate;
    return var;
}

bool Db_Ota::modifyItem(const sOtaItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);
    query.bindValue(":dtime",item.datetime);
    query.bindValue(":ver",item.ver);
    query.bindValue(":md5",item.md5);
    query.bindValue(":remark",item.remark);
    query.bindValue(":oldVersion",item.oldVersion);
    query.bindValue(":releaseDate",item.releaseDate);

    return sqlQuery(query);
}

void Db_Ota::selectItem(QSqlQuery &query,sOtaItem &item)
{
    item.id = query.value("id").toInt();
    item.datetime = query.value("dtime").toString();
    item.ver = query.value("ver").toString();
    item.md5 = query.value("md5").toString();
    item.remark = query.value("remark").toString();
    item.oldVersion = query.value("oldVersion").toString();
    item.releaseDate = query.value("releaseDate").toString();
}
