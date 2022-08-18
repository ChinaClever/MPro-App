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

    QVariant pduGetParam(uchar type, uchar fc, uchar id=0, uchar sub=0);
    bool pduSetParam(uchar type, uchar fc, const QVariant &value, uchar id=0, uchar sub=0);
    bool pduRelaysCtrl(int addr, int start, int num, uchar on);
    QString pduLogFun(uchar type, uchar fc, int id, int noe=30);
};

#endif // SSHRPCCLIENT_H
