#ifndef RPC_SERVICE_H
#define RPC_SERVICE_H

#include "rpc_core.h"
struct sRpcCfg{
    int en = 0; // 0 禁用 1 TCP  2 websocket
    int port = 6002;
};

class Rpc_Service : public JsonRpcObj
{
    Q_OBJECT
    explicit Rpc_Service(QObject *parent = nullptr);
public:
    static Rpc_Service *bulid(QObject *parent = nullptr);    
    static sRpcCfg rpcCfg;
    bool startRpc(int en);
    bool setPort(int port);

private slots:
    void initFunSlot();

};

#endif // RPC_SERVICE_H
