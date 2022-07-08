#ifndef IPC_LOGCLIENT_H
#define IPC_LOGCLIENT_H

#include "ipc_echoclient.h"

class IPC_LogClient : public IPC_EchoClient
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
