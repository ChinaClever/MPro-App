#ifndef PDURPCOBJ_H
#define PDURPCOBJ_H
#include "jsonrpcobj.h"

class PduRpcObj
{
public:
    static void rpc_export();
    static char *pduReadData(struct mg_str mg_r);
    static char *pduSetData(struct mg_str r);
    static char *pduReadString(struct mg_str r);
    static char *pduSetString(struct mg_str r);

    static char *pduReadCfg(mg_str r);
    static char *pduSetCfg(struct mg_str r);
    static char *pduLogFun(struct mg_str r);

protected:
    static char *responRpcData(const QVector<double> &its, double value);
    static char *responRpcString(const QVector<double> &its, QString value);

protected:
    static IPC_WebClient *mWebIpc;
};

#endif // PDURPCOBJ_H
