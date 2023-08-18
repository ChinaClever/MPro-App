/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "odbc_obj.h"

sOdbcCfg Odbc_Obj::cfg;
Odbc_Obj::Odbc_Obj()
{
    mDb = QSqlDatabase::addDatabase("QMYSQL", ODBC_NAME);
}

bool Odbc_Obj::throwError(const QString &db, const QSqlError &err)
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
    } if(!ret) qCritical() << "odbc sql error: " << db << str << err.text();
    return ret;
}

bool Odbc_Obj::db_open()
{
    bool ret = false;
    QSqlDatabase *db = &mDb;
    if(db->isValid() && cfg.en) {
        db->setPort(cfg.port);
        db->setHostName(cfg.host);
        db->setDatabaseName(cfg.db);
        db->setUserName(cfg.user);
        db->setPassword(cfg.pwd);

        ret = db->open();
        if(!ret) ret = db->open();
        if(ret) cfg.okCnt++; // qDebug() << "odbc connect ok" <<
        else {cfg.errCnt++, throwError(ODBC_NAME, db->lastError());}
        cfg.status = ret;
    }

    return ret;
}


bool Odbc_Obj::db_transaction()
{
    bool ret = true; isDbRun = true;
    //qDebug() << "odbc transaction" << QTime::currentTime().toString("hh:mm:ss zzz");
    if (mDb.driver()->hasFeature(QSqlDriver::Transactions)) {
        ret = mDb.transaction(); //qDebug() << "odbc transaction" << ret;
    } else {
        mDb.exec("START TRANSACTION");
        qDebug() << "odbc not transaction" << ret;
    }
    return ret;
}

bool Odbc_Obj::db_commit()
{
    // qDebug() << "odbc commit" << QTime::currentTime().toString("hh:mm:ss zzz"); mDb.commit();
    //qDebug() << "odbc close" << QTime::currentTime().toString("hh:mm:ss zzz"); mDb.close();
    bool ret = true; if (mDb.driver()->hasFeature(QSqlDriver::Transactions)) {
        ret = mDb.commit(); if(!ret) mDb.rollback();
    } else {
        cout << ret;
        //ret = mDb.exec("COMMIT"); if(!ret) sqlQuery("ROLLBACK");
    } isDbRun = false; cm::mdelay(143);
    return ret;
}
