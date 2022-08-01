#ifndef IPC_LOGCLIENT_H
#define IPC_LOGCLIENT_H

#include "ipc_objclient.h"
enum eLogs{
    eUserLog,
    eAlarmLog,
    eOpLog,
    eSysLog,
    eEleLog,
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

class IPC_LogClient : public IPC_ObjClient
{
    Q_OBJECT
public:
    explicit IPC_LogClient(QObject *parent = nullptr);
    QVariant log_fun(uchar type, uchar fc, uint id, uint cnt);

private:
    QString log_get(const sLogFcIt &unit);
    bool log_msgSend(const sLogFcIt &msg);
};

#endif // IPC_LOGCLIENT_H
