#ifndef IPC_ECHOCLIENT_H
#define IPC_ECHOCLIENT_H

#include "ipc_cfgclient.h"

class IPC_EchoClient : public IPC_ObjClient
{
public:
    explicit IPC_EchoClient(QObject *parent = nullptr);

    QString getString(uchar addr, uchar fc, uchar id);
    bool setString(uint addr, uchar fc, uchar id, const QString &str);

    int getDevCfg(uchar addr, uchar fc, uchar type);
    bool setDevCfg(uint addr, uchar fc, uchar type, int value);

    int getValue(uchar addr, uchar type, uchar topic, uchar sub, uchar id);
    bool setting(uint addr, uchar type, uchar topic, uchar sub, uchar id, uint value);

private:
    bool getValue(sDataItem &unit);
    QString getNumStr(sNumStrItem &unit);

    bool setting(const sDataItem &unit);
    bool setNumStr(const sNumStrItem &unit);

    bool msgSend(int fc, const QByteArray &msg);
};

#endif // IPC_ECHOCLIENT_H
