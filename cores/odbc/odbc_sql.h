#ifndef ODBC_SQL_H
#define ODBC_SQL_H
#include "odbc_obj.h"

class Odbc_Sql : public Odbc_Obj
{
public:
    Odbc_Sql();

    bool sqlQuery(QSqlQuery &query);
    bool sqlQuery(const QString &sql);
    bool sqlQuery(QSqlQuery &query, const QString &sql);
    int cntBySql(const QString &sql);
};

#endif // ODBC_SQL_H
