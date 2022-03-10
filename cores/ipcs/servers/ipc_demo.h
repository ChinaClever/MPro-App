#ifndef IPC_DEMO_H
#define IPC_DEMO_H

#include "ipc_objserver.h"

class IPC_Demo : public IPC_ObjServer
{
    Q_OBJECT
    explicit IPC_Demo(QObject *parent = nullptr);
public:
    static IPC_Demo *bulid(QObject *parent = nullptr);

public slots:
    void dbus_recv_slot(const int &v, const int &i);
    QStringList dbus_reply_slot(const int& i, const QString& v);

protected:
    QList<const char *> busRecvMethods();
    QByteArray lsRecv(const QByteArray &v);
};

void ipc_demo(QObject *p);

#endif // IPC_DEMO_H
