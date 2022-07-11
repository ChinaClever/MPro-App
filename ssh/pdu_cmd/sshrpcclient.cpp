/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "sshrpcclient.h"

SshRpcClient::SshRpcClient(QObject *parent)
    : JsonRpcObj{parent}
{
    startClient("127.0.0.1", 6004);
}

SshRpcClient *SshRpcClient::bulid(QObject *parent)
{
    static SshRpcClient* sington = nullptr;
    if(sington == nullptr) {
        sington = new SshRpcClient(parent);
    }
    return sington;
}


int SshRpcClient::pduMetaData(uchar addr,  uchar type, uchar topic, uchar sub, uchar id)
{
    int ret = -1;
    auto result = rpc_client->call("pduMetaData", addr, type, topic, sub, id);
    if (result->isSuccess()) {
        ret = result->result().toInt();
    } else {
        qDebug() << Q_FUNC_INFO << "RPC error:" << result->toString();
    }

    return ret;
}

bool SshRpcClient::pduSetData(uchar addr,  uchar type, uchar topic, uchar sub, uchar id, uint value, uchar soi)
{
    bool ret = false;
    auto result = rpc_client->call("pduSetData", addr, type, topic,sub, id, value, soi);
    if (result->isSuccess()) {
        ret = result->result().toBool();
    } else {
        qDebug() << Q_FUNC_INFO << "RPC error:" << result->toString();
    }

    return ret;
}

QString SshRpcClient::pduGetString(uchar addr, uchar fc, uchar id)
{
    QString str;
    auto result = rpc_client->call("pduGetString", addr, fc, id);
    if (result->isSuccess()) {
        str = result->result().toString();
    } else {
        qDebug() << Q_FUNC_INFO << "RPC error:" << result->toString();
    }
    return str;
}

bool SshRpcClient::pduSetString(uchar addr, uchar fc, uchar id, const QString &str, uchar soi)
{
    bool ret = false;
    auto result = rpc_client->call("pduSetString", addr, fc, id, str, soi);
    if (result->isSuccess()) {
        ret = result->result().toBool();
    } else {
        qDebug() << Q_FUNC_INFO << "RPC error:" << result->toString();
    }

    return ret;
}

int SshRpcClient::pduDevCfg(uchar addr, uchar fc, uchar type)
{
    int ret = -1;
    auto result = rpc_client->call("pduDevCfg", addr, fc, type);
    if (result->isSuccess()) {
        ret = result->result().toInt();
    } else {
        qDebug() << Q_FUNC_INFO << "RPC error:" << result->toString();
    }

    return ret;
}

bool SshRpcClient::pduSetCfg(uchar addr, uchar fc, uchar type, int value, uchar soi)
{
    bool ret = false;
    auto result = rpc_client->call("pduSetCfg", addr, fc, type, value, soi);
    if (result->isSuccess()) {
        ret = result->result().toBool();
    } else {
        qDebug() << Q_FUNC_INFO << "RPC error:" << result->toString();
    }

    return ret;
}

QString SshRpcClient::pduLogFun(uchar type, uchar fc, int id, int noe)
{
    QString str;
    auto result = rpc_client->call("pduLogFun", type, fc, id, noe);
    if (result->isSuccess()) {
        str = result->result().toString();
    } else {
        qDebug() << Q_FUNC_INFO << "RPC error:" << result->toString();
    }
    return str;
}
