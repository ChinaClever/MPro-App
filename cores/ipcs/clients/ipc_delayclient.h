#ifndef IPC_DELAYCLIENT_H
#define IPC_DELAYCLIENT_H

#include "ipc_outputclient.h"

class IPC_DelayClient : public IPC_ObjClient
{
    Q_OBJECT
    explicit IPC_DelayClient(QObject *parent = nullptr);
public:
    static IPC_DelayClient *bulid(QObject *parent = nullptr);
    bool setDelay(int addr, int id, uchar fc, uchar delay);

signals:

private:
    bool inputCheck(const QVariantList &values);
};

#endif // IPC_DELAYCLIENT_H
