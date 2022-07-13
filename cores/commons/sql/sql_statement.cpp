/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "sql_statement.h"
#include "cfg_obj.h"

Sql_Statement::Sql_Statement()
{
    initDb();
    initTableMarking();
}

bool Sql_Statement::initTableMarking()
{
    QString sql = "create table if not exists markingtable("
                  "name TEXT primary key not null, marking TEXT not null);";
    return sqlQuery(sql);
}

/**
 * @brief 异常
 * @param err
 */
bool Sql_Statement::throwError(const QSqlError &err)
{
    QString str;
    bool ret = false;
    if(err.isValid()) {     //发生错误时isValid()返回true
        switch (err.type()) {
        case QSqlError::NoError: str = QObject::tr("无错误"); ret = true; break;
        case QSqlError::UnknownError: str = QObject::tr("未知错误"); break;
        case QSqlError::ConnectionError: str = QObject::tr("连接错误");break;
        case QSqlError::StatementError: str = QObject::tr("SQL语句错误"); break;
        case QSqlError::TransactionError: str = QObject::tr("事务错误"); break;
        }
    }
    if(!ret) qCritical() << "Sql_Error: " << QObject::tr("数据库名：")
                         << tableName() << str << err.text();
    return ret;
}

bool Sql_Statement::sqlQuery(QSqlQuery &query)
{
    query.setForwardOnly(true); bool ret = query.exec();
    if(!ret) ret = throwError(query.lastError());
    return ret;
}
bool Sql_Statement::sqlQuery(QSqlQuery &query, const QString &sql)
{
    bool ret = query.prepare(sql);
    if(ret) ret = sqlQuery(query);
    if(!ret) qDebug() << sql;
    return ret;
}

bool Sql_Statement::sqlQuery(const QString &sql)
{
    QSqlQuery query(mDb);
    return sqlQuery(query, sql);
}

/**
 * @brief 删除
 * @param condition
 */
bool Sql_Statement::remove(const QString &condition)
{
    QString sql = "DELETE FROM %1 WHERE %2";
    return sqlQuery(sql.arg(tableName()).arg(condition));
}

bool Sql_Statement::remove(const QString &name, const QString &vale)
{
    QString sql = "%1 = \'%2\'";
    return remove(sql.arg(name, vale));
}

bool Sql_Statement::remove(int id)
{
    return remove(QString("id = %1").arg(id));
}

bool Sql_Statement::removeMinIds(int id)
{
    return remove(QString("id < %1").arg(id));
}

bool Sql_Statement::removeMaxIds(int id)
{
    return remove(QString("id > %1").arg(id));
}

int Sql_Statement::idBySql(const QString &sql)
{
    int id = 0; QSqlQuery query(mDb);
    if(sqlQuery(query, sql)) {
        if(query.next()) id = query.value(0).toInt();
    }
    return id;
}

int Sql_Statement::maxId(const QString &column_name, const QString &condition)
{
    QString sql = "select max(%1) from %2 %3";
    return idBySql(sql.arg(column_name, tableName(), condition));
}

int Sql_Statement::maxId()
{
    return maxId("id", "");
}

int Sql_Statement::maxId(const QString &condition)
{
    return maxId("id", QString("where %1").arg(condition));
}

int Sql_Statement::minId(const QString &column_name, const QString &condition)
{
    QString sql = "select MIN(%1) from %2 %3";
    return idBySql(sql.arg(column_name, tableName(), condition));
}

int Sql_Statement::minId(const QString &condition)
{
    return minId("id", QString("where %1").arg(condition));
}

int Sql_Statement::minId()
{
    return minId("id", "");
}

/**
 * @brief 函数返回指定列的值的数目
 * @param column_name
 * @return
 */
int Sql_Statement::counts(const QString &column_name, const QString &condition)
{
    QString sql = "select count(DISTINCT %1) from %2 %3";
    return idBySql(sql.arg(column_name, tableName(), condition));
}

int Sql_Statement::counts(const QString &name)
{
    return counts(name, "");
}

int Sql_Statement::counts()
{
    return counts("id");
}

int Sql_Statement::contains(const QString &name, const QString &value)
{
    QString condition = QString("where %1=\'%2\'").arg(name).arg(value);
    return counts(name, condition);
}

/**
 * @brief 数据库最多保存多少条数据，删除掉其它的
 * @param count
 * @return
 */
bool Sql_Statement::countsRemove(int count)
{
    bool ret = false;
    int rtn = counts(); if(rtn > count) {
        int id = minId() + (rtn-count);
        ret = removeMinIds(id);
    }

    return ret;
}

/**
 * @brief 函数返回列的所有值
 * @param column_name 列名
 * @param condition 条件
 * @return
 */
QVariantList Sql_Statement::listColumn(const QString &column_name, const QString &condition)
{
    QVariantList list; QSqlQuery query(mDb);
    QString sql = QString("select DISTINCT %1 from %2 %3")
            .arg(column_name, tableName(), condition);
    if(sqlQuery(query, sql)) {
        while(query.next()) list << query.value(0);
    }

    return list;
}

QVariantList Sql_Statement::selectIds(const QString &condition)
{
    return listColumn("id", condition);
}

bool Sql_Statement::updateColumn(const QString& column_name, double value, const QString &condition)
{
    QString sql = QString("update %1 set %2=%3 %4")
            .arg(tableName(), column_name).arg(value).arg(condition);
    return sqlQuery(sql);
}

bool Sql_Statement::updateColumn(const QString& column_name, QString value, const QString &condition)
{
    QString sql = QString("update  %1 set %2=\'%3\' where %4")
            .arg(tableName(), column_name, value, condition);
    return sqlQuery(sql);
}

bool Sql_Statement::clear()
{
    QString sql = "delete from %1";
    return sqlQuery(sql.arg(tableName()));;
}

QString Sql_Statement::tableMarking()
{
    QString ret; QSqlQuery query(mDb);
    QString sql = QString("SELECT * from markingtable where name = \'%1\'").arg(tableName());
    if(sqlQuery(query,sql)){
        if(query.next()) ret = query.value(1).toString();
    }
    return ret;
}

void Sql_Statement::setTableMarking(const QString &marking)
{
    QString ori = tableMarking();
    QString cmd = ori.isEmpty()
            ? "insert into markingtable (name,marking) values(%1,%2)"
            : "update markingtable set marking = %2 where name = \"%1\"";
    sqlQuery(cmd.arg(tableName(),marking));
}

bool Sql_Statement::createTable()
{
    QString val = "id INTEGER primary key autoincrement not null, dtime VCHAR,";
    for(int i=1; i<headList().size(); ++i) val += headList().at(i) + " VCHAR,";
    val.chop(1); QString sql = "create table if not exists %1(%2);";
    return sqlQuery(sql.arg(tableName(), val));
}

/**
 * @brief 数据库初始化
 */
bool Sql_Statement::initDb()
{
    mDb = QSqlDatabase::database();
    bool ret = mDb.isOpen(); if(!ret){
        mDb = QSqlDatabase::addDatabase("QSQLITE");
        mDb.setDatabaseName(Cfg_Obj::pathOfCfg("logs.db"));
        ret = mDb.open(); if(!ret) throwError(mDb.lastError());
    }
    return ret;
}
