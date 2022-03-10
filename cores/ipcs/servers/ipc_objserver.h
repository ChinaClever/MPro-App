#ifndef IPC_OBJSERVER_H
#define IPC_OBJSERVER_H

#include "ipc_objclient.h"

class IPC_ObjServer : public LSocket_Server
{
    Q_OBJECT
public:
    explicit IPC_ObjServer(QObject *parent = nullptr);
     sDataPacket *dataPacket();

protected:
    virtual void initFunction(const QString &key);
};

#endif // IPC_OBJSERVER_H
