/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "rpc_service.h"
#include <QTimer>

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

void Rpc_Service::initFunSlot()
{
    startServer({new Rpc_Core(DTxType::TxRpc)});
    startLocalServer({new Rpc_Core(DTxType::TxSsh)});
}
