#ifndef JSONRPCCLIENT_H
#define JSONRPCCLIENT_H

#include "jsonrpcobj.h"

class JsonRpcClient : public JsonRpcObj
{
    Q_OBJECT
public:
    explicit JsonRpcClient(QObject *parent = nullptr);

    void invokeMethodSync();
    void invokeStringMethodSync();
    void invokeNotification();

signals:

private:

};

#endif // JSONRPCCLIENT_H
