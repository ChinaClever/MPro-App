#ifndef PDURPCOBJ_H
#define PDURPCOBJ_H
#include "jsonrpcobj.h"

class PduRpcObj
{
public:
    static void rpc_export();
    static void pduReadData(struct jsonrpc_request *r);
    static void pduSetData(struct jsonrpc_request *r);

private:
    static void responRpc(jsonrpc_request *r, const QVector<double> &its, double value);
};

#endif // PDURPCOBJ_H
