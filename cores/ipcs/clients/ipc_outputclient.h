#ifndef IPC_OUTPUTCLIENT_H
#define IPC_OUTPUTCLIENT_H

#include "ipc_logclient.h"

class IPC_OutputClient : public IPC_LogClient
{
public:
    explicit IPC_OutputClient(QObject *parent = nullptr);
    bool relayCtrl(uchar addr, uchar id, uchar value);
    bool relayDelay(uchar addr, uchar id, uchar value);
    bool relayMode(uchar addr, uchar id, uchar value);
    bool opNameSet(int addr, int id, const QString &value);
    QString opName(int addr, int id);

private:
    bool relaySet(uchar addr, uchar id, uchar fc, uchar value);
};

#endif // IPC_OUTPUTCLIENT_H
