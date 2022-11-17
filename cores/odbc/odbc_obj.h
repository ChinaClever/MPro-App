#ifndef ODBC_OBJ_H
#define ODBC_OBJ_H
#include <QtCore>
#include <QtSql>
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
    }

    bool en;
    int port;
    QString host;
    QString user;
    QString pwd;
    QString db;
    bool status;
    int dataPoll;
    int hdaPoll;
};

class Odbc_Obj
{
public:
    Odbc_Obj();
    static sOdbcCfg cfg;

    bool db_open();
    bool throwError(const QSqlError &err);

protected:
    QSqlDatabase mDb;
};

#endif // ODBC_OBJ_H
