#ifndef RPC_OBJ_H
#define RPC_OBJ_H
#include "set_core.h"
#include "jsonrpcobj.h"

class Rpc_Obj : public QObject
{
    Q_OBJECT
public:
    explicit Rpc_Obj(QObject *parent = nullptr);
    Q_INVOKABLE int pduMetaData(uchar addr,  uchar type, uchar topic, uchar sub, uchar id);
    Q_INVOKABLE bool pduSetData(uchar addr,  uchar type, uchar topic, uchar sub, uchar id, uint value);

    Q_INVOKABLE QString pduGetString(uchar addr, uchar fc, uchar id);
    Q_INVOKABLE bool pduSetString(uchar addr, uchar fc, uchar id, const QString &str);

signals:
    void sendUnsolicitedNotification(const QString&, const QVariant&);

private:
    sDataItem mIt;
};

#endif // RPC_OBJ_H
