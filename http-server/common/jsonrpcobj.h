#ifndef JSONRPCOBJ_H
#define JSONRPCOBJ_H
#include <QtCore>
#include "mjson.h"
#include "mongoose.h"
#include "ipc_webclient.h"

class JsonRpcObj
{
public:
    static double getNumber(jsonrpc_request* r, int id);
    static QVector<double> getNumbers(jsonrpc_request* r, int num);
    static bool getString(jsonrpc_request* r, int id, char *s);
};

#endif // JSONRPCOBJ_H
