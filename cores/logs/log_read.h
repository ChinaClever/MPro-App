#ifndef LOG_READ_H
#define LOG_READ_H

#include "db_alarm.h"
#include "datapacket.h"

enum eLogs{
    eUserLog,
    eAlarmLog,
    eOpLog,
    eSysLog,
    eEleLog,
    eHardwareLog,
    eOtaLog,
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


class Log_Read : public QObject
{
    Q_OBJECT
public:
    explicit Log_Read(QObject *parent = nullptr);
    QString log_readFun(const sLogFcIt &it);
    QString logFun(const sCfgItem &cfg);

private:
    Sql_Statement *getSql(int type);
    QString log_readOnce(int type, int id);
    QString log_readPage(int type, int id, int cnt);
};

#endif // LOG_READ_H
