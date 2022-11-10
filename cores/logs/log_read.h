#ifndef LOG_READ_H
#define LOG_READ_H

#include "log_sys.h"

enum eLogs{
    eUserLog,
    eAlarmLog,
    eEventLog,
    eOtaLog,
    eHdaLog,
};

enum eLogFc {
    eLog_cnt=1,
    eLog_read,
    eLog_clear,
    eLog_readOnce,
};

struct sLogFcIt {
    sLogFcIt() {type=fc=0; id=0;cnt=30;}
    uchar type;
    uchar fc;
    uint id;
    uint cnt;
};

struct sLogHdaIt
{
    sLogHdaIt() {addr=type=topic=index=0;}
    int addr, type, topic, index;
    QString start, end;
};


class Log_Read : public Log_Sys
{
public:
    explicit Log_Read(QObject *parent = nullptr);
    QString log_readHda(const sLogHdaIt &it);
    QString log_readFun(const sLogFcIt &it);
    QString logFun(const sCfgItem &cfg);

private:
    Sql_Statement *getSql(int type);
    QString log_readOnce(int type, int id);
    QString log_readPage(int type, int id, int cnt);

protected:
    QReadWriteLock *mRwLock;
};

#endif // LOG_READ_H
