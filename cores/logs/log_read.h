#ifndef LOG_READ_H
#define LOG_READ_H

#include "db_alarm.h"
#include "ipc_logclient.h"

class Log_Read : public QObject
{
    Q_OBJECT
public:
    explicit Log_Read(QObject *parent = nullptr);

    QString log_readFun(const sLogFcIt &it);

private:
    Sql_Statement *getSql(int type);
    QString log_readOnce(int type, int id);
    QString log_readPage(int type, int page, int cnt);
};

#endif // LOG_READ_H
