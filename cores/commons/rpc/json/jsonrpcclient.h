#ifndef JSONRPCCLIENT_H
#define JSONRPCCLIENT_H

#include "jsonrpcobj.h"

class JsonRpcClient : public JsonRpcObj
{
    Q_OBJECT
public:
    explicit JsonRpcClient(QObject *parent = nullptr);
    int pduGetData(uchar addr,  uchar type, uchar topic, uchar sub, uchar id);
    bool pduSetData(uchar addr,  uchar type, uchar topic, uchar sub, uchar id, uint value);

    QVariant pduGetParam(uchar type, uchar fc, uchar addr=0, uchar sub=0);
    bool pduSetParam(uchar type, uchar fc, const QVariant &value, uchar addr=0, uchar sub=0);
    bool pduRelaysCtrl(int addr, int start, int num, uchar on);
    QString pduLogFun(uchar type, uchar fc, int id, int cnt=30);
    QString pduMetaData(uchar addr);
};

#endif // JSONRPCCLIENT_H
