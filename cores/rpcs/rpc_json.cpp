/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "rpc_json.h"

Rpc_Json::Rpc_Json(QObject *parent)
    : QObject{parent}
{
    mMethod = Rpc_Method::bulid(this);
    mServer = new JsonRpc_Server(this);
    mSsh = new JsonRpc_Server(this);
    mSsh->startLocalServer({mMethod}, 9224);

    mWeb = new JsonRpc_Server(this);
    mWeb->startLocalServer({mMethod}, 9225);
}

bool Rpc_Json::startRpc(int en, int port)
{
    SocketType socket_type;
    switch (en) {
    case 1: socket_type = SocketType::tcp; break;
    case 2: socket_type = SocketType::websocket; break;
    default: mServer->close(); return true;
    }

    return mServer->startServer({mMethod}, port, socket_type);
}
