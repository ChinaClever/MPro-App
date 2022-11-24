#ifndef ODBC_EVENT_H
#define ODBC_EVENT_H
#include "odbc_hda.h"

struct sOdbcEventIt
{
    sOdbcEventIt() {}
    int addr = -1;
    QString event_type;
    QString event_content;
};

class Odbc_Event : public Odbc_Hda
{
public:
    Odbc_Event();
protected:
    bool event_createTable();
    bool event_insert(const sOdbcEventIt &it);
private:
    bool event_modifyItem(const sOdbcEventIt &it, const QString &fmd);
};

#endif // ODBC_EVENT_H
