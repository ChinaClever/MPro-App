#ifndef IPC_ECHOCLIENT_H
#define IPC_ECHOCLIENT_H

#include "ipc_cfgclient.h"

class IPC_EchoClient : public IPC_ObjClient
{
public:
    explicit IPC_EchoClient(QObject *parent = nullptr);

    QString getString(uchar addr, uchar fc, uchar id);
    bool setString(uchar addr, uchar fc, uchar id, const QString &str);

    int getValue(uchar addr, uchar type, uchar topic, uchar sub, uchar id);
    bool setting(uchar addr, uchar type, uchar topic, uchar sub, uchar id, uint value);

private:
    bool getValue(sDataItem &unit);
    QString getString(sStrItem &unit);

    bool setting(const sDataItem &unit);
    bool setString(const sStrItem &unit);

    bool msgSend(int fc, const QByteArray &msg);
};

#endif // IPC_ECHOCLIENT_H
