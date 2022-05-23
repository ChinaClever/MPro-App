#ifndef RPC_READ_H
#define RPC_READ_H
#include "set_core.h"
#include "jsonrpcobj.h"


class Rpc_Read : public QObject
{
    Q_OBJECT
public:
    explicit Rpc_Read(QObject *parent = nullptr);
    Q_INVOKABLE uint pduMetaData(uchar addr,  uchar type, uchar topic, uchar sub, uchar id);

    Q_INVOKABLE QString printMessage(const QString& msg);
    Q_INVOKABLE void printNotification(const QString& msg);
    Q_INVOKABLE void namedParams(QString& msg, int answer);

private:

signals:
    void sendUnsolicitedNotification(const QString&, const QVariant&);

private:
    sDataItem mIt;
};

#endif // RPC_READ_H
