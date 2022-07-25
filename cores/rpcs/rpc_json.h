#ifndef RPC_JSON_H
#define RPC_JSON_H

#include "rpc_method.h"
#include "jsonrpcobj.h"

class Rpc_Json : public JsonRpcObj
{
    Q_OBJECT
public:
    explicit Rpc_Json(QObject *parent = nullptr);
    bool startRpc(int en, int port);

private:
    Rpc_Method *mMethod;
};

#endif // RPC_JSON_H
