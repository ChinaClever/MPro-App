#include "db_ele.h"

Db_Ele::Db_Ele()
{
    mHeadList << "addr" << "module" << "ele";
}

Db_Ele *Db_Ele::bulid()
{
    static Db_Ele* sington = nullptr;
    if(!sington) {
        sington = new Db_Ele();
        sington->createTable();
    }
    return sington;
}

QJsonArray Db_Ele::itemJson(const sEleItem &it)
{
    QJsonArray  var;
    var << it.datetime << it.addr;
    var << it.module << it.ele;
    return var;
}

bool Db_Ele::modifyItem(const sEleItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);
    query.bindValue(":dtime",item.datetime);
    query.bindValue(":addr",item.addr);
    query.bindValue(":module",item.module);
    query.bindValue(":ele",item.ele);
    return sqlQuery(query);
}

void Db_Ele::selectItem(QSqlQuery &query,sEleItem &item)
{
    item.id = query.value("id").toInt();
    item.datetime = query.value("dtime").toString();
    item.addr = query.value("addr").toString();
    item.module = query.value("module").toString();
    item.ele = query.value("ele").toString();
}
