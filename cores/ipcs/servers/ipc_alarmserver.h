#ifndef IPC_ALARMSERVER_H
#define IPC_ALARMSERVER_H

#include "ipc_delayserver.h"

class IPC_AlarmServer : public IPC_ObjServer
{
    Q_OBJECT
    explicit IPC_AlarmServer(QObject *parent = nullptr);
public:
    static IPC_AlarmServer *bulid(QObject *parent = nullptr);

public slots:
    void dbus_recv_slot(const QByteArray &array);

protected:
    QList<const char *> busRecvMethods();
};

#endif // IPC_ALARMSERVER_H
