#include "db_op.h"


Db_Op::Db_Op()
{
    mHeadList << "op_src"<< "content";
}

Db_Op *Db_Op::bulid()
{
    static Db_Op* sington = nullptr;
    if(!sington) {
        sington = new Db_Op();
        sington->createTable();
    }
    return sington;
}

QJsonArray Db_Op::itemJson(const sOpItem &it)
{
    QJsonArray  var;
    var << it.datetime << it.op_src;
    var << it.content;
    return var;
}

bool Db_Op::modifyItem(const sOpItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);
    query.bindValue(":dtime",item.datetime);
    query.bindValue(":op_src",item.op_src);
    query.bindValue(":content",item.content);
    return sqlQuery(query);
}

void Db_Op::selectItem(QSqlQuery &query,sOpItem &item)
{
    item.id = query.value("id").toInt();
    item.datetime = query.value("dtime").toString();
    item.op_src = query.value("op_src").toString();
    item.content = query.value("content").toString();
}
