/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "odbc_obj.h"

sOdbcCfg Odbc_Obj::cfg;
Odbc_Obj::Odbc_Obj()
{

}

bool Odbc_Obj::throwError(const QSqlError &err)
{
    bool ret = false;
    QString str; if(err.isValid()) {     //发生错误时isValid()返回true
        switch (err.type()) {
        case QSqlError::NoError: str = "NoError"; ret = true; break;
        case QSqlError::UnknownError: str = "UnknownError"; break;
        case QSqlError::ConnectionError: str = "ConnectionError";break;
        case QSqlError::StatementError: str = "StatementError"; break;
        case QSqlError::TransactionError: str = "TransactionError"; break;
        }
    } if(!ret) qCritical() << "odbc sql rrror: " << str << err.text();
    return ret;
}

bool Odbc_Obj::db_open()
{
    mDb = QSqlDatabase::addDatabase("QMYSQL", ODBC_NAME);
    bool ret = false; QSqlDatabase *db = &mDb;
    if(db->isValid() && cfg.en) {
        db->setHostName(cfg.host);
        db->setPort(cfg.port);
        db->setDatabaseName(cfg.db);
        db->setUserName(cfg.user);
        db->setPassword(cfg.pwd);
        ret = cfg.status = db->open();
        if(ret) qDebug() << "odbc connect ok";
        else throwError(db->lastError());
    }

    return ret;
}
