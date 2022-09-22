/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "jsonrpcclient.h"

JsonRpcClient::JsonRpcClient(QObject *parent)
    : JsonRpcObj{parent}
{

}


QString JsonRpcClient::pduMetaData(uchar addr)
{
    QString ret;
    auto result = rpc_client->call("pduMetaData", addr);
    if (result->isSuccess()) {
        ret = result->result().toString();
    } else {
        qDebug() << Q_FUNC_INFO << "RPC error:" << result->toString();
    }

    return ret;
}

int JsonRpcClient::pduGetData(uchar addr,  uchar type, uchar topic, uchar sub, uchar id)
{
    int ret = -1;
    auto result = rpc_client->call("pduGetData", addr, type, topic, sub, id);
    if (result->isSuccess()) {
        ret = result->result().toInt();
    } else {
        qDebug() << Q_FUNC_INFO << "RPC error:" << result->toString();
    }

    return ret;
}

bool JsonRpcClient::pduSetData(uchar addr,  uchar type, uchar topic, uchar sub, uchar id, uint value)
{
    bool ret = false;
    auto result = rpc_client->call("pduSetData", addr, type, topic,sub, id, value);
    if (result->isSuccess()) {
        ret = result->result().toBool();
    } else {
        qDebug() << Q_FUNC_INFO << "RPC error:" << result->toString();
    }

    return ret;
}

QVariant JsonRpcClient::pduGetParam(uchar type, uchar fc, uchar addr, uchar sub)
{
    QVariant ret;
    auto result = rpc_client->call("pduGetParam", type, fc, addr, sub);
    if (result->isSuccess()) {
        ret = result->result();
    } else {
        qDebug() << Q_FUNC_INFO << "RPC error:" << result->toString();
    }

    return ret;
}

bool JsonRpcClient::pduSetParam(uchar type, uchar fc, const QVariant &value, uchar addr, uchar sub)
{
    bool ret = false;
    auto result = rpc_client->call("pduSetParam", type, fc, value, addr, sub);
    if (result->isSuccess()) {
        ret = result->result().toBool();
    } else {
        qDebug() << Q_FUNC_INFO << "RPC error:" << result->toString();
    }

    return ret;
}

bool JsonRpcClient::pduRelaysCtrl(int addr, int start, int num, uchar on)
{
    bool ret = false;
    auto result = rpc_client->call("pduRelaysCtrl", addr, start, num, on);
    if (result->isSuccess()) {
        ret = result->result().toBool();
    } else {
        qDebug() << Q_FUNC_INFO << "RPC error:" << result->toString();
    }

    return ret;
}

QString JsonRpcClient::pduLogFun(uchar type, uchar fc, int id, int cnt)
{
    QString str;
    auto result = rpc_client->call("pduLogFun", type, fc, id, cnt);
    if (result->isSuccess()) {
        str = result->result().toString();
    } else {
        qDebug() << Q_FUNC_INFO << "RPC error:" << result->toString();
    }
    return str;
}
