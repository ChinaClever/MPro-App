#ifndef ODBC_OBJ_H
#define ODBC_OBJ_H
#include <QtSql>
#include "commons.h"
#define ODBC_NAME "mysql"

struct sOdbcCfg
{
    sOdbcCfg() {
        en = 1;
        host = "192.168.1.102";
        port = 3306;
        db = "pdu";
        user = "pdu";
        pwd = "123456";
        pdukey = "pdu";
        dataPoll = 15;
        hdaPoll = 1;
    }

    int en;
    int port;
    bool status=0;
    QString host;
    QString user;
    QString pwd;
    QString db;
    QString pdukey;
    int dataPoll;
    int hdaPoll;
    uint okCnt=0;
    uint errCnt=0;
};

class Odbc_Obj
{
public:
    Odbc_Obj();
    static sOdbcCfg cfg;

    bool db_open();
    bool db_commit();
    bool db_transaction();
    void addCnt() {mCnt++;}
    void db_close(){db_commit(); mDb.close();}
    bool throwError(const QString &db, const QSqlError &err);

protected:
    uint mCnt = 1;
    QSqlDatabase mDb;
    bool isDbRun=false;
};

#endif // ODBC_OBJ_H
