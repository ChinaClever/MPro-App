/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "rpc_service.h"
#include <QTimer>

sRpcCfg Rpc_Service::rpcCfg;
Rpc_Service::Rpc_Service(QObject *parent)
    : QObject{parent}
{
    mXml = new Rpc_Xml(this);
    mJson = new Rpc_Json(this);
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

bool Rpc_Service::startJsonRpc()
{
    sRpcIt *ptr = &rpcCfg.json;
    return mJson->startRpc(ptr->en, ptr->port);
}

bool Rpc_Service::startXmlRpc()
{
    sRpcIt *ptr = &rpcCfg.xml;
    return mXml->startXmlRpc(ptr->en, ptr->port);
}

void Rpc_Service::initFunSlot()
{
    mJson->startRpc(rpcCfg.json.en, rpcCfg.json.port);
    mXml->startXmlRpc(rpcCfg.xml.en, rpcCfg.xml.port);
}
