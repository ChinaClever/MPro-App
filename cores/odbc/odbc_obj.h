#ifndef ODBC_OBJ_H
#define ODBC_OBJ_H
#include <QtSql>
#include "commons.h"
#define ODBC_NAME "mysql"

struct sOdbcCfg
{
    sOdbcCfg() {
        en = true;
        host = "192.168.1.102";
        port = 3306;
        db = "pdu";
        user = "pdu";
        pwd = "123456";
        pdukey = "pdu";
    }

    bool en;
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
    void db_close();
    void addCnt() {mCnt++;}
    bool throwError(const QSqlError &err);

protected:
    uint mCnt = 1;
    QSqlDatabase mDb;
};

#endif // ODBC_OBJ_H
