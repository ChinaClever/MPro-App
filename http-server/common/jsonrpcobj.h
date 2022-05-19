#ifndef JSONRPCOBJ_H
#define JSONRPCOBJ_H
#include <stdarg.h>
#include <stdlib.h>
#include "mjson.h"
#include <QtCore>

//using
class JsonRpcObj
{
public:
    static double getNumber(jsonrpc_request* r, int id);
    static QVector<double> getNumbers(jsonrpc_request* r, int num);
    static bool getString(jsonrpc_request* r, int id, char *s);
};

#endif // JSONRPCOBJ_H
