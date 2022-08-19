#ifndef WEB_RPC_H
#define WEB_RPC_H
#include "web_obj.h"

class Web_Rpc
{
public:
    Web_Rpc();

protected:
    static char *pduMetaData(mg_str &mg_r);
    static char *pduReadData(mg_str &r);
    static char *pduSetData(mg_str &r);
    static char *pduReadParam(mg_str &r);
    static char *pduSetParam(mg_str &r);
    static char *pduLogFun(mg_str &r);

private:
    static char *responRpcData(const QVector<uint> &ls, double value);
    static char *responRpcString(const QVector<uint> &ls, const QString &value);

private:
    static Web_Obj *mObj;
};

#endif // WEB_RPC_H
