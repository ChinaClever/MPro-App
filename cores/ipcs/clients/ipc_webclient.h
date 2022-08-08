#ifndef IPC_WEBCLIENT_H
#define IPC_WEBCLIENT_H

#include "ipc_logclient.h"

class IPC_WebClient : public IPC_LogClient
{
    Q_OBJECT
    explicit IPC_WebClient(QObject *parent = nullptr);
public:
    static IPC_WebClient *bulid(QObject *parent = nullptr);
    QString getCfg(uchar addr, uchar fc, uchar id);
    bool setCfg(uint addr, uchar fc, uchar type, const QVariant &value);

    int getValue(uchar addr, uchar type, uchar topic, uchar sub, uchar id);
    bool setting(uint addr, uchar type, uchar topic, uchar sub, uchar id, uint value);

private:
    bool getValue(sDataItem &unit);
    bool setting(const sDataItem &unit);
    bool msgSend(int fc, const QByteArray &msg);
};

#endif // IPC_WEBCLIENT_H
