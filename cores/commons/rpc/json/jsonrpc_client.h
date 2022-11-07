#ifndef JSONRPC_CLIENT_H
#define JSONRPC_CLIENT_H
#include "jsonrpc_server.h"
#include "jcon/json_rpc_tcp_client.h"
#include "jcon/json_rpc_websocket_client.h"
#include <QtCore>

class JsonRpc_Client : public QObject
{
    Q_OBJECT
public:
    explicit JsonRpc_Client(QObject *parent = nullptr);
    bool startClient(const QString& host, int port=6002,
                     SocketType socket_type = SocketType::tcp);
    bool startLocalClient(int port);
    bool isConnected();
    ~JsonRpc_Client();
    void close();

    double pduDataGet(uchar addr,  uchar type, uchar topic, uchar sub, uchar id);
    bool pduDataSet(uchar addr,  uchar type, uchar topic, uchar sub, uchar id, double value);
    QString pduLogHda(const QString &start, const QString &end, int addr, int type, int topic, int index);

    QVariant pduCfgGet(uchar type, uchar fc, uchar id=0, uchar addr=0);
    bool pduCfgSet(uchar type, uchar fc, const QVariant &value, uchar id=0, uchar addr=0);
    bool pduRelaysCtrl(int addr, int start, int num, uchar on);
    QString pduLogFun(uchar type, uchar fc, int id, int cnt=30);
    QString execute(const QString &cmd);
    QString pduMetaData(uchar addr);
    void initReceiveNotification();

private slots:
    void timeoutDone();

private:
    int mPort = 0;
    QTimer *timer = nullptr;
    jcon::JsonRpcClient* rpc_client = nullptr;
};

#endif // JSONRPC_CLIENT_H
