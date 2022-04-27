#ifndef IPC_DELAYCLIENT_H
#define IPC_DELAYCLIENT_H

#include "ipc_relayclient.h"

class IPC_DelayClient : public IPC_ObjClient
{
    Q_OBJECT
    explicit IPC_DelayClient(QObject *parent = nullptr);
public:
    static IPC_DelayClient *bulid(QObject *parent = nullptr);
    bool setDelay(int addr, int id, uchar mode, uchar delay);

signals:

private:
    bool inputCheck(const QVariantList &values);
};

#endif // IPC_DELAYCLIENT_H
