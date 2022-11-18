/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "odbc_sql.h"

Odbc_Sql::Odbc_Sql()
{

}


bool Odbc_Sql::sqlQuery(QSqlQuery &query)
{
    query.setForwardOnly(true); bool ret = query.exec();
    if(!ret) ret = throwError(query.lastError());
    return ret;
}

bool Odbc_Sql::sqlQuery(QSqlQuery &query, const QString &sql)
{
    bool ret = query.prepare(sql);
    if(ret) ret = sqlQuery(query);
    if(!ret) qDebug() << sql << throwError(query.lastError());
    return ret;
}

bool Odbc_Sql::sqlQuery(const QString &sql)
{
    QSqlQuery query(mDb);
    return sqlQuery(query, sql);
}
