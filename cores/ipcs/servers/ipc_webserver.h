#ifndef IPC_WEBSERVER_H
#define IPC_WEBSERVER_H

#include "ipc_logserver.h"

class IPC_WebServer : public IPC_LogServer
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", DBUS_SERVICE_NAME".webs")
    explicit IPC_WebServer(QObject *parent = nullptr);
public:
    static IPC_WebServer *bulid(QObject *parent = nullptr);

public slots:
    void dbus_recv_slot(int fc, const QByteArray &array);
    QString dbus_reply_slot(int fc, const QByteArray &array);

protected:
    QList<const char *> busRecvMethods();
    QByteArray lsRecv(const QByteArray &v);
};

#endif // IPC_WEBSERVER_H
