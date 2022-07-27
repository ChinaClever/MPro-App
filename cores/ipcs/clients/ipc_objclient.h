#ifndef IPC_OBJCLIENT_H
#define IPC_OBJCLIENT_H

#include "domain_socketcli.h"

class IPC_ObjClient : public QObject
{
    Q_OBJECT
public:
    explicit IPC_ObjClient(QObject *parent = nullptr);
    sDataPacket *dataPacket();

protected:
    bool sendSocket(const QVariantList &v);
    QVariant readSocket(const QVariantList &v, int msec=3);
    virtual bool inputCheck(const QVariantList &){return true;}

private:
    Domain_SocketCli *mSocket;
};

#endif // IPC_OBJCLIENT_H
