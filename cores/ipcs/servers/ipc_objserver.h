#ifndef IPC_OBJSERVER_H
#define IPC_OBJSERVER_H
#include "set_core.h"
#include "ipc_objclient.h"

class IPC_ObjServer : public Domain_SocketServ
{
    Q_OBJECT
public:
    explicit IPC_ObjServer(QObject *parent = nullptr);

protected:
    virtual QVariant ipc_recv_msg(int fc, const QByteArray &array) = 0;

private:
     QVariant ipc_reply(const QByteArray &array) override;
};

#endif // IPC_OBJSERVER_H
