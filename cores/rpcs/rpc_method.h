#ifndef RPC_METHOD_H
#define RPC_METHOD_H
#include "set_core.h"


class Rpc_Method : public QObject
{
    Q_OBJECT
    explicit Rpc_Method(QObject *parent = nullptr);
public:
    static Rpc_Method *bulid(QObject *parent = nullptr);
    //void setTxType(uchar txType) {mTxType=txType;}

public slots:
    Q_INVOKABLE double pduDataGet(int addr, int type, int topic, int sub, int id);
    Q_INVOKABLE int pduDataSet(int addr, int type, int topic, int sub, int id, double value, int txType=0);
    Q_INVOKABLE int pduCfgSet(int type, int fc, const QVariant &value, int id=0, int addr=0, int txType=0);
    Q_INVOKABLE QString pduLogHda(const QString &start, const QString &end, int addr, int type, int topic, int index);
    Q_INVOKABLE QString pduCfgGet(int type, int fc, int id=0, int addr=0);
    Q_INVOKABLE QString pduLogFun(int type, int fc, int id=0, int cnt=0);
    Q_INVOKABLE bool pduRelaysCtrl(int addr, int start, int num, int on, int txType=0);
    Q_INVOKABLE QString pduMetaData(int addr=0, int dc=0);
    Q_INVOKABLE QString execute(const QString &cmd);

signals:
    void sendUnsolicitedNotification(const QString&, const QVariant&);

private:
    //uchar mTxType=0;
    sDataItem mIt;
};

#endif // RPC_METHOD_H
