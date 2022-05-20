#include "jsonrpcclient.h"

JsonRpcClient::JsonRpcClient(QObject *parent)
    : JsonRpcObj{parent}
{

}


void JsonRpcClient::invokeMethodSync()
{
    qsrand(std::time(nullptr));

    auto result = rpc_client->call("pduMetaData", 0, 1,2,3);

    if (result->isSuccess()) {
        qDebug() << "result of synchronous RPC call:" << result->result();
    } else {
        qDebug() << "RPC error:" << result->toString();
    }
}

void JsonRpcClient::invokeStringMethodSync()
{
    qsrand(std::time(nullptr));

    auto result = rpc_client->call("printMessage", "hello, world");

    if (result->isSuccess()) {
        qDebug() << "result of synchronous RPC call:" << result->result();
    } else {
        qDebug() << "RPC error:" << result->toString();
    }
}

void JsonRpcClient::invokeNotification()
{
    rpc_client->notification("printNotification", "hello, notification");
}
