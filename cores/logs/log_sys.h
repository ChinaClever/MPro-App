#ifndef LOG_SYS_H
#define LOG_SYS_H

#include "db_alarm.h"
#include "datapacket.h"

struct sSysLogCfg
{
    int en;
    QString host;
    int port;
};

class Log_Sys : public QObject
{
public:
    explicit Log_Sys(QObject *parent = nullptr);
    static sSysLogCfg sysLogCfg;
    void sys_log(const QString& msg);

protected:
    void sys_initfun();
};

#endif // LOG_SYS_H
