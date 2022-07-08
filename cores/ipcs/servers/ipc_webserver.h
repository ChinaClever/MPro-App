#ifndef IPC_WEBSERVER_H
#define IPC_WEBSERVER_H

#include "ipc_logserver.h"

class IPC_WebServer : public IPC_LogServer
{
    Q_OBJECT
    explicit IPC_WebServer(QObject *parent = nullptr);
public:
    static IPC_WebServer *bulid(QObject *parent = nullptr);

private:
    QVariant ipc_recv_msg(int fc, const QByteArray &array) override;
};

#endif // IPC_WEBSERVER_H
