#ifndef JSONRPCOBJ_H
#define JSONRPCOBJ_H
#include <QtCore>
#include "mongoose.h"
#include "commons.h"
#include "ipc_webclient.h"

class JsonRpcObj
{
public:
    static double getNumber(mg_str &r, int id);
    static QVector<double> getNumbers(mg_str &r, int num);
    static QString getString(mg_str &r, int id);
};

#endif // JSONRPCOBJ_H
