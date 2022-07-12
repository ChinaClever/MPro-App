#ifndef IPC_LCDSERVER_H
#define IPC_LCDSERVER_H

#include "ipc_webserver.h"

class IPC_LcdServer : public Domain_SocketServ
{
    Q_OBJECT
    explicit IPC_LcdServer(QObject *parent = nullptr);
public:
     static IPC_LcdServer *bulid(QObject *parent = nullptr);
private:
     QVariant ipc_reply(const QByteArray &array) override;
};

#endif // IPC_LCDSERVER_H
