#ifndef PDURPCOBJ_H
#define PDURPCOBJ_H
#include "jsonrpcobj.h"

class PduRpcObj
{
public:
    static void rpc_export();
    static void pduReadData(struct jsonrpc_request *r);
    static void pduSetData(struct jsonrpc_request *r);

protected:
    static void responRpc(jsonrpc_request *r, const QVector<double> &its, double value);

protected:
    static IPC_WebClient *mWebIpc;
};

#endif // PDURPCOBJ_H
