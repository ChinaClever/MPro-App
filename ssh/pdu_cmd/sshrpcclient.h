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

    QVariant pduGetParam(uchar addr, uchar fc, uchar type);
    bool pduSetParam(uchar addr, uchar fc, uchar type, const QVariant &value);
    bool pduRelaysCtrl(int addr, int start, int num, uchar on);
    QString pduLogFun(uchar type, uchar fc, int id, int noe=30);
};

#endif // SSHRPCCLIENT_H
