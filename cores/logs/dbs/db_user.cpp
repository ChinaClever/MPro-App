/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "db_user.h"

Db_User::Db_User()
{
    mHeadList << "name"<< "pwd"<< "jurisdiction"<< "email"
              << "telephone"<<"remarks"<< "jur";
}

Db_User *Db_User::bulid()
{
    static Db_User* sington = nullptr;
    if(!sington) {
        sington = new Db_User();
        sington->createTable();
    }
    return sington;
}

QJsonArray Db_User::itemJson(const sUserItem &it)
{
    QJsonArray  var;
    var << it.datetime << it.name;
    var << it.pwd << it.email << it.telephone;
    return var;
}

bool Db_User::modifyItem(const sUserItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);
    query.bindValue(":dtime",item.datetime);
    query.bindValue(":name",item.name);
    query.bindValue(":pwd",item.pwd);
    query.bindValue(":email",item.email);
    query.bindValue(":jurisdiction",item.jurisdiction);
    query.bindValue(":telephone",item.telephone);
    query.bindValue(":remarks",item.remarks);
    query.bindValue(":jur",item.jur);
    return sqlQuery(query);
}

void Db_User::selectItem(QSqlQuery &query,sUserItem &item)
{
    item.id = query.value("id").toInt();
    item.datetime = query.value("dtime").toString();
    item.name = query.value("name").toString();
    item.pwd = query.value("pwd").toString();
    item.email = query.value("email").toString();
    item.jurisdiction = query.value("jurisdiction").toString();
    item.telephone = query.value("telephone").toString();
    item.remarks = query.value("remarks").toString();
    item.jur = query.value("jur").toInt();
}

sUserItem Db_User::selItemsByName(const QString& name)
{
    QVector<sUserItem> items = selectItems("name", name);
    return items.first();
}

void Db_User::removeItemsByName(const QString& name)
{
    remove("name", name);
}

int Db_User::contain(const QString &name)
{
    return contains("name", name);
}
