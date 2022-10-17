#ifndef JSONRPC_SERVER_H
#define JSONRPC_SERVER_H

#include "jcon/json_rpc_tcp_server.h"
#include "jcon/json_rpc_websocket_server.h"

enum class SocketType {tcp, websocket};
class JsonRpc_Server : public QObject
{
public:
    explicit JsonRpc_Server(QObject *parent = nullptr);
    bool startLocalServer(const QObjectList &services, int port);
    bool startServer(const QObjectList &services, int port=6002,
                     SocketType socket_type = SocketType::tcp);
    void close();
    ~JsonRpc_Server();

private:
    jcon::JsonRpcServer* rpc_server = nullptr;
};

#endif // JSONRPC_SERVER_H
