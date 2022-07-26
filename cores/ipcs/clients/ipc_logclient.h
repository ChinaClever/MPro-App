#ifndef IPC_LOGCLIENT_H
#define IPC_LOGCLIENT_H

#include "ipc_objclient.h"
enum eLogs{
    eUserLog,
    eAlarmLog,
    eOpLog,
    eSysLog,
    eEleLog,
};

enum eLogFc {
    eLog_cnt=1,
    eLog_read,
    eLog_page,
    eLog_clear,
};

struct sLogFcIt {
    sLogFcIt() {type=fc=0; id=0;noe=30;}
    uchar type;
    uchar fc;
    uchar noe;
    uint id;
};

class IPC_LogClient : public IPC_ObjClient
{
    Q_OBJECT
public:
    explicit IPC_LogClient(QObject *parent = nullptr);
    QVariant log_fun(uchar type, uchar fc, uint id, uchar noe);

private:
    QString log_get(const sLogFcIt &unit);
    bool log_msgSend(const sLogFcIt &msg);
};

#endif // IPC_LOGCLIENT_H
