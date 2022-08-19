#ifndef RPC_METHOD_H
#define RPC_METHOD_H
#include "set_core.h"


class Rpc_Method : public QObject
{
    Q_OBJECT
    explicit Rpc_Method(QObject *parent = nullptr);
public:
    static Rpc_Method *bulid(QObject *parent = nullptr);
    void setTxType(uchar txType) {mTxType=txType;}

public slots:
    Q_INVOKABLE int pduGetData(uchar addr, uchar type, uchar topic, uchar sub, uchar id);
    Q_INVOKABLE bool pduSetData(uchar addr, uchar type, uchar topic, uchar sub, uchar id, uint value);
    Q_INVOKABLE bool pduSetParam(uchar type, uchar fc, const QVariant &value, uchar addr=0, uchar sub=0);
    Q_INVOKABLE QString pduGetParam(uchar type, uchar fc, uchar addr=0, uchar sub=0);
    Q_INVOKABLE QString pduLogFun(uchar type, uchar fc, int id=0, int cnt=0);
    Q_INVOKABLE bool pduRelaysCtrl(int addr, int start, int num, uchar on);
    Q_INVOKABLE QString pduMetaData(uchar addr=0);

signals:
    void sendUnsolicitedNotification(const QString&, const QVariant&);

private:
    uchar mTxType=0;
    sDataItem mIt;
};

#endif // RPC_METHOD_H
