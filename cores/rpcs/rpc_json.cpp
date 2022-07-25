/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "rpc_json.h"

Rpc_Json::Rpc_Json(QObject *parent)
    : JsonRpcObj{parent}
{
     mMethod = Rpc_Method::bulid(this);
     startLocalServer({mMethod});
}

bool Rpc_Json::startRpc(int en, int port)
{
    SocketType socket_type;
    switch (en) {
    case 1: socket_type = SocketType::tcp; break;
    case 2: socket_type = SocketType::websocket; break;
    default: close(); return true;
    }

    return startServer({mMethod}, port, socket_type);
}
