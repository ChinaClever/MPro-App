#ifndef ODBC_ALARM_H
#define ODBC_ALARM_H
#include "odbc_data.h"

struct sOdbcAlarmIt
{
    sOdbcAlarmIt() {}
    QString alarm_status;
    QString alarm_content;
};

class Odbc_Alarm : public Odbc_Data
{
public:
    Odbc_Alarm();

protected:
    bool alarm_createTable();
    bool alarm_insert(const sOdbcAlarmIt &it);

private:
    bool alarm_modifyItem(const sOdbcAlarmIt &it, const QString &fmd);
};

#endif // ODBC_ALARM_H
