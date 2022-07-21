#ifndef SSHRPCCLIENT_H
#define SSHRPCCLIENT_H

#include "jsonrpcobj.h"

class SshRpcClient : public JsonRpcObj
{
    explicit SshRpcClient(QObject *parent = nullptr);
public:
    static SshRpcClient *bulid(QObject *parent = nullptr);
    int pduMetaData(uchar addr,  uchar type, uchar topic, uchar sub, uchar id);
    bool pduSetData(uchar addr,  uchar type, uchar topic, uchar sub, uchar id, uint value, uchar soi=0);

    QString pduGetString(uchar addr, uchar fc, uchar id);
    bool pduSetString(uchar addr, uchar fc, uchar id, const QString &str, uchar soi=0);

    int pduDevCfg(uchar addr, uchar fc, uchar type);
    bool pduSetCfg(uchar addr, uchar fc, uchar type, int value, uchar soi=0);

    bool pduRelaysCtrl(int addr, int start, int num, uchar on);
    QString pduLogFun(uchar type, uchar fc, int id, int noe=30);
};

#endif // SSHRPCCLIENT_H
