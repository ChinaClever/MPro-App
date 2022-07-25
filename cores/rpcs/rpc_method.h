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
    Q_INVOKABLE int pduMetaData(uchar addr, uchar type, uchar topic, uchar sub, uchar id);
    Q_INVOKABLE bool pduSetData(uchar addr, uchar type, uchar topic, uchar sub, uchar id,
                                uint value, uchar soi=0);

    Q_INVOKABLE QString pduGetString(uchar addr, uchar fc, uchar id);
    Q_INVOKABLE bool pduSetString(uchar addr, uchar fc, uchar id,
                                  const QString &str, uchar soi=0);

    Q_INVOKABLE int pduDevCfg(uchar addr, uchar fc, uchar type);
    Q_INVOKABLE bool pduSetCfg(uchar addr, uchar fc, uchar type, int value, uchar soi=0);
    Q_INVOKABLE QString pduLogFun(uchar type, uchar fc, int id, int noe=30);

    Q_INVOKABLE uint pduRelayStatus(int addr, int id);
    Q_INVOKABLE bool pduRelayCtrl(int addr, int id, uchar on);
    Q_INVOKABLE bool pduGroupCtrl(int addr, int id, uchar on);
    Q_INVOKABLE bool pduRelayModeSet(int addr, int id, uchar mode);
    Q_INVOKABLE bool pduRelayDelaySet(int addr, int id, uchar delay);
    Q_INVOKABLE bool pduRelaysCtrl(int addr, int start, int num, uchar on);
    Q_INVOKABLE bool pduGroupingSet(int addr, int id, const QString &value);
    Q_INVOKABLE bool pduGroupNameSet(int addr, int id, const QString &value);
    Q_INVOKABLE bool pduOutputNameSet(int addr, int id, const QString &value);
    Q_INVOKABLE QString pduOutputNameGet(int addr, int id);
    Q_INVOKABLE QString pduGroupNameGet(int addr, int id);
    Q_INVOKABLE QString pduGroupingGet(int addr, int id);

    Q_INVOKABLE QString pduDevNameGet(int addr);
    Q_INVOKABLE bool pduDevNameSet(int addr, const QString &value);

    Q_INVOKABLE QString pduDevSnGet(int addr);
    Q_INVOKABLE bool pduDevSnSet(int addr, const QString &value);
    Q_INVOKABLE bool pduQRcodeGenerator(const QString &value, int addr);

signals:
    void sendUnsolicitedNotification(const QString&, const QVariant&);

private:
    uchar mTxType=0;
    sDataItem mIt;
};

#endif // RPC_METHOD_H
