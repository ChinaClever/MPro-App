#ifndef PDURPCOBJ_H
#define PDURPCOBJ_H
#include "jsonrpcobj.h"

class PduRpcObj
{
public:
    static void rpc_export();
    static char *pduReadData(mg_str &mg_r);
    static char *pduSetData(mg_str &r);
    static char *pduReadString(mg_str &r);
    static char *pduSetString(mg_str &r);

    static char *pduReadCfg(mg_str &r);
    static char *pduSetCfg(mg_str &r);
    static char *pduLogFun(mg_str &r);

protected:
    static char *responRpcData(const QVector<double> &its, double value);
    static char *responRpcString(const QVector<double> &its, const QString &value);

protected:
    static IPC_WebClient *mWebIpc;
};

#endif // PDURPCOBJ_H
