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
    Q_INVOKABLE double pduDataGet(int addr, int type, int topic, int sub, int id);
    Q_INVOKABLE bool pduDataSet(int addr, int type, int topic, int sub, int id, double value);
    Q_INVOKABLE bool pduCfgSet(int type, int fc, const QVariant &value, int id=0, int addr=0);
    Q_INVOKABLE QString pduCfgGet(int type, int fc, int id=0, int addr=0);
    Q_INVOKABLE QString pduLogFun(int type, int fc, int id=0, int cnt=0);
    Q_INVOKABLE bool pduRelaysCtrl(int addr, int start, int num, int on);
    Q_INVOKABLE QString pduMetaData(int addr=0);
    Q_INVOKABLE QString execute(const QString &cmd);

signals:
    void sendUnsolicitedNotification(const QString&, const QVariant&);

private:
    double getDecimal(const sDataItem &it);

private:
    uchar mTxType=0;
    sDataItem mIt;
};

#endif // RPC_METHOD_H
