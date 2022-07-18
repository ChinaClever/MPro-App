/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "rpc_service.h"
#include <QTimer>

sRpcCfg Rpc_Service::rpcCfg;
Rpc_Service::Rpc_Service(QObject *parent)
    : JsonRpcObj{parent}
{
    QTimer::singleShot(550,this,SLOT(initFunSlot()));
}

Rpc_Service *Rpc_Service::bulid(QObject *parent)
{
    static Rpc_Service* sington = nullptr;
    if(sington == nullptr) {
        sington = new Rpc_Service(parent);
    }
    return sington;
}

bool Rpc_Service::startRpc(int en)
{
    SocketType socket_type;
    rpcCfg.en = en; switch (en) {
    case 1: socket_type = SocketType::tcp; break;
    case 2: socket_type = SocketType::websocket; break;
    default: close(); return true;
    }
    return startServer({new Rpc_Core(DTxType::TxRpc)}, rpcCfg.port, socket_type);
}

bool Rpc_Service::setPort(int port)
{
    rpcCfg.port = port;
    return startRpc(rpcCfg.en);
}

void Rpc_Service::initFunSlot()
{
    startRpc(rpcCfg.en);
    startLocalServer({new Rpc_Core(DTxType::TxSsh)});
}
