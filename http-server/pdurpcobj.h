#ifndef PDURPCOBJ_H
#define PDURPCOBJ_H
#include "jsonrpcobj.h"

class PduRpcObj
{
public:
    static void rpc_export();
    static void pduReadData(struct jsonrpc_request *r);
    static void pduSetData(struct jsonrpc_request *r);
    static void pduReadString(struct jsonrpc_request *r);
    static void pduSetString(struct jsonrpc_request *r);

protected:
    static void responRpcData(jsonrpc_request *r, const QVector<double> &its, double value);
    static void responRpcString(jsonrpc_request *r, const QVector<double> &its, QString value);

protected:
    static IPC_WebClient *mWebIpc;
};

#endif // PDURPCOBJ_H
