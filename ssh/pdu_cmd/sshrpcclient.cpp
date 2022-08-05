/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "sshrpcclient.h"

SshRpcClient::SshRpcClient(QObject *parent)
    : JsonRpcObj{parent}
{
    bool ret = startClient("127.0.0.1", 9224);
    if(!ret) qDebug() << "SSH Client connected err";
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

bool SshRpcClient::pduSetData(uchar addr,  uchar type, uchar topic, uchar sub, uchar id, uint value)
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

QVariant SshRpcClient::pduGetParam(uchar addr, uchar fc, uchar type)
{
    QVariant ret;
    auto result = rpc_client->call("pduGetParam", addr, fc, type);
    if (result->isSuccess()) {
        ret = result->result();
    } else {
        qDebug() << Q_FUNC_INFO << "RPC error:" << result->toString();
    }

    return ret;
}

bool SshRpcClient::pduSetParam(uchar addr, uchar fc, uchar type, const QVariant &value)
{
    bool ret = false;
    auto result = rpc_client->call("pduSetParam", addr, fc, type, value);
    if (result->isSuccess()) {
        ret = result->result().toBool();
    } else {
        qDebug() << Q_FUNC_INFO << "RPC error:" << result->toString();
    }

    return ret;
}

bool SshRpcClient::pduRelaysCtrl(int addr, int start, int num, uchar on)
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
