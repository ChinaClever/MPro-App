#ifndef IPC_OBJCLIENT_H
#define IPC_OBJCLIENT_H

#include "lsocket_server.h"

#define IPC_KEY_LOG "logs"
#define IPC_KEY_SETTING "settings"

class IPC_ObjClient : public QObject
{
    Q_OBJECT
public:
    explicit IPC_ObjClient(QObject *parent = nullptr);
    sDataPacket *dataPacket();

protected:
    QVariant readBus(const QVariantList &v);
    virtual void initFunction(const QString &key);
    virtual bool inputCheck(const QVariantList &values) = 0;

protected :
    DBus_Call *mDbus;
};

#endif // IPC_OBJCLIENT_H
