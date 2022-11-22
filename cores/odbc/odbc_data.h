#ifndef ODBC_DATA_H
#define ODBC_DATA_H
#include "odbc_event.h"

struct sOdbcDataIt {
    uchar addr;
    uchar type;
    uchar topic;
    uchar indexes;
    double value;
};

class Odbc_Data : public Odbc_Event
{
public:
    Odbc_Data();

protected:
    bool data_createTable();
    bool data_poll(const sOdbcDataIt &it);

private:
    int data_counts(const sOdbcDataIt &it);
    bool data_update(const sOdbcDataIt &it);
    bool data_insert(const sOdbcDataIt &it);
    bool data_modifyItem(const sOdbcDataIt &it, const QString &fmd);

private:
    bool mDataInit=false;
};

#endif // ODBC_DATA_H
