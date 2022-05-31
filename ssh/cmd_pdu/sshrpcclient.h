#ifndef SSHRPCCLIENT_H
#define SSHRPCCLIENT_H

#include "jsonrpcobj.h"

class SshRpcClient : public JsonRpcObj
{
    explicit SshRpcClient(QObject *parent = nullptr);
public:
    static SshRpcClient *bulid(QObject *parent = nullptr);
    int pduMetaData(uchar addr,  uchar type, uchar topic, uchar sub, uchar id);
    bool pduSetData(uchar addr,  uchar type, uchar topic, uchar sub, uchar id, uint value);

    QString pduGetString(uchar addr, uchar fc, uchar id);
    bool pduSetString(uchar addr, uchar fc, uchar id, const QString &str);
};

#endif // SSHRPCCLIENT_H
