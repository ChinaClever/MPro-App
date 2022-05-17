#ifndef IPC_OUTPUTSERVER_H
#define IPC_OUTPUTSERVER_H

#include "ipc_objserver.h"

class IPC_OutputServer : public IPC_ObjServer
{
    Q_OBJECT
    explicit IPC_OutputServer(QObject *parent = nullptr);
public:
    static IPC_OutputServer *bulid(QObject *parent = nullptr);

public slots:
    void dbus_recv_slot(int addr, int id, int fc, const QString &msg);

protected:
    QList<const char *> busRecvMethods();
};

#endif // IPC_OUTPUTSERVER_H
