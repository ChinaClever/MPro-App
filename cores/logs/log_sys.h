#ifndef LOG_SYS_H
#define LOG_SYS_H

#include "db_ota.h"
#include "datapacket.h"

struct sSysLogCfg
{
    int en;
    QString host;
    int port;
};

class Log_Sys : public QThread
{
public:
    explicit Log_Sys(QObject *parent = nullptr);
    static sSysLogCfg sysLogCfg;
    void sys_logAlarm(const sAlarmItem &it);
    void sys_logInfo(const sEventItem &it);

public slots:
    void sys_open();

protected:
    void sys_initfun();
    QReadWriteLock *mRwLock;
};

QReadWriteLock *log_rwLock();
#endif // LOG_SYS_H
