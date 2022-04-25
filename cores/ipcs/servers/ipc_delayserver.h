#ifndef IPC_DELAYSERVER_H
#define IPC_DELAYSERVER_H

#include "ipc_relayserver.h"

class IPC_DelayServer : public IPC_ObjServer
{
    Q_OBJECT
    explicit IPC_DelayServer(QObject *parent = nullptr);
public:
    static IPC_DelayServer *bulid(QObject *parent = nullptr);

public slots:
    void dbus_recv_slot(int addr, int id, int mode, int delay);

protected:
    QList<const char *> busRecvMethods();
};

#endif // IPC_DELAYSERVER_H
