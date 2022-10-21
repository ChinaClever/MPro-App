/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "jsonrpc_server.h"

JsonRpc_Server::JsonRpc_Server(QObject *parent)
    : QObject{parent}
{

}

JsonRpc_Server::~JsonRpc_Server()
{
    close();
}

void JsonRpc_Server::close()
{
    if(rpc_server) rpc_server->close();
}

bool JsonRpc_Server::startServer(const QObjectList& services, int port, SocketType socket_type)
{
    if(rpc_server) {close(); delete rpc_server;}
    if (socket_type == SocketType::tcp) {
        rpc_server = new jcon::JsonRpcTcpServer(this);
    } else {
        rpc_server = new jcon::JsonRpcWebSocketServer(this);
    }
    rpc_server->enableSendNotification(true);
    rpc_server->registerServices(services);
    return rpc_server->listen(port);
}

bool JsonRpc_Server::startLocalServer(const QObjectList &services, int port)
{
    rpc_server = new jcon::JsonRpcTcpServer(this);
    rpc_server->enableSendNotification(true);
    rpc_server->registerServices(services);
    return rpc_server->listen(QHostAddress::LocalHost, port);
}
