#ifndef ODBC_INDEX_H
#define ODBC_INDEX_H
#include "odbc_sql.h"

struct sOdbcIndexIt {
    int id = -1;
    QString uuid;
    QString dev_key;
    int run_status;
    QString current_alarm;
};


class Odbc_Index : public Odbc_Sql
{
public:
    Odbc_Index();

protected:
    bool index_createTable();
    uint getPduId(const QString &uuid);
    uint getPduId(int addr);
    void index_polls();

private:
    int index_counts(const sOdbcIndexIt &it);
    bool index_insert(const sOdbcIndexIt &it);
    bool index_update(const sOdbcIndexIt &it);
    bool index_counts(const QString &uuit, const QString &key);
    bool index_modifyItem(const sOdbcIndexIt &it, const QString &fmd);
    bool index_poll(const QString &uuid);
    uint index_uuid(const QString &uuid);
    bool index_update(int addr);

private:
    QString m_addr;
    QMap<QString, uint> mKeys;
};

#endif // ODBC_INDEX_H
