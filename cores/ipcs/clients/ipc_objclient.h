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

    int getValue(sDataItem &unit);
    bool setting(const sDataItem &unit);

    bool setString(const sStrItem &unit);
    bool setString(uchar addr, uchar fc, uchar id, const QString &str);

    int getValue(uchar addr, uchar type, uchar topic, uchar sub, uchar id);
    bool setting(uchar addr, uchar type, uchar topic, uchar sub, uchar id, uint value);

protected:
    QVariant readBus(const QVariantList &v);
    bool msgSend(int fc, const QByteArray &msg);
    virtual void initFunction(const QString &key, bool f=false);
    virtual bool inputCheck(const QVariantList &){return true;}

protected :
    DBus_Call *mDbus;
};

#endif // IPC_OBJCLIENT_H
