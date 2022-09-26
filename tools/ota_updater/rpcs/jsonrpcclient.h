#ifndef JSONRPCCLIENT_H
#define JSONRPCCLIENT_H

#include "jsonrpcobj.h"

class JsonRpcClient : public JsonRpcObj
{
    Q_OBJECT
public:
    explicit JsonRpcClient(QObject *parent = nullptr);

    double pduMetaData(uchar addr,  uchar type, uchar topic, uchar sub, uchar id);
    bool pduDataSet(uchar addr,  uchar type, uchar topic, uchar sub, uchar id, double value);

    QString pduGetString(uchar addr, uchar fc, uchar id);
    bool pduSetString(uchar addr, uchar fc, uchar id, const QString &str);
    QString pduLogFun(uchar type, uchar fc, int id, int cnt);


    void invokeStringMethodSync();
    void invokeNotification();

signals:

private:

};

#endif // JSONRPCCLIENT_H
