#ifndef PDURPCOBJ_H
#define PDURPCOBJ_H
#include "webrpcobj.h"

class PduRpcObj
{
public:
    static void rpc_export();
    static char *pduReadData(mg_str &mg_r);
    static char *pduSetData(mg_str &r);

    static char *pduReadParam(mg_str &r);
    static char *pduSetParam(mg_str &r);
    static char *pduLogFun(mg_str &r);


    static char *pduReadString(mg_str &r);
    static char *pduSetString(mg_str &r);
    static char *pduReadCfg(mg_str &r);
    static char *pduSetCfg(mg_str &r);

protected:
    static char *responRpcData(const QVector<double> &its, double value);
    static char *responRpcString(const QVector<double> &its, const QString &value);

protected:
    static WebRpcObj *mObj;
    static IPC_WebClient *mWebIpc;
};

#endif // PDURPCOBJ_H
