#ifndef IPC_RELAYSERVER_H
#define IPC_RELAYSERVER_H

#include "ipc_objserver.h"

class IPC_RelayServer : public IPC_ObjServer
{
    Q_OBJECT
    explicit IPC_RelayServer(QObject *parent = nullptr);
public:
    static IPC_RelayServer *bulid(QObject *parent = nullptr);

public slots:
    void dbus_recv_slot(int addr, int id, int value);

protected:
    QList<const char *> busRecvMethods();
};

#endif // IPC_RELAYSERVER_H
