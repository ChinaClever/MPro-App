#ifndef IPC_OBJCLIENT_H
#define IPC_OBJCLIENT_H

#include "lsocket_server.h"

#define IPC_KEY_LOG         "logs"
#define IPC_KEY_RELAY       "relays"
#define IPC_KEY_DELAY       "delays"
#define IPC_KEY_WEB         "webs"
#define IPC_KEY_SETTING     "settings"

enum eLogs{
    eUserLog,
    eOpLog,
    eSysLog,
    eAlarmLog,

};

class IPC_ObjClient : public QObject
{
    Q_OBJECT
public:
    explicit IPC_ObjClient(QObject *parent = nullptr);
    sDataPacket *dataPacket();

protected:
    QVariant readBus(const QVariantList &v);
    virtual void initFunction(const QString &key, bool f=false);
    virtual bool inputCheck(const QVariantList &){return true;}

protected :
    DBus_Call *mDbus;
};

#endif // IPC_OBJCLIENT_H
