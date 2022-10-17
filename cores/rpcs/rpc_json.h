#ifndef RPC_JSON_H
#define RPC_JSON_H

#include "rpc_method.h"
#include "jsonrpc_server.h"

class Rpc_Json : public QObject
{
    Q_OBJECT
public:
    explicit Rpc_Json(QObject *parent = nullptr);
    bool startRpc(int en, int port);

private:
    Rpc_Method *mMethod;
    JsonRpc_Server *mSsh;
    JsonRpc_Server *mWeb;
    JsonRpc_Server *mServer;
};

#endif // RPC_JSON_H
