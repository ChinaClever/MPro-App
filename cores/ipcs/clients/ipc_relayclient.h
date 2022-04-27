#ifndef IPC_RELAYCLIENT_H
#define IPC_RELAYCLIENT_H

#include "ipc_cfgclient.h"

class IPC_RelayClient : public IPC_ObjClient
{
    Q_OBJECT
    explicit IPC_RelayClient(QObject *parent = nullptr);
public:
    static IPC_RelayClient *bulid(QObject *parent = nullptr);
    bool ctrl(int addr, int id, int value);

signals:

private:
    bool inputCheck(const QVariantList &values);
};

#endif // IPC_RELAYCLIENT_H
