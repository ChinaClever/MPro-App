/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "jsonrpc_client.h"

JsonRpc_Client::JsonRpc_Client(QObject *parent)
    : QObject{parent}
{
    timer = new QTimer(this);
    timer->start(3500 + rand()%100);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
}

JsonRpc_Client::~JsonRpc_Client()
{
    close();
}

QString JsonRpc_Client::execute(const QString &cmd)
{
    QString ret; if(!isConnected()) return ret;
    auto result = rpc_client->call("execute", cmd);
    if (result->isSuccess()) {
        ret = result->result().toString();
    } else {
        qDebug() << Q_FUNC_INFO << "RPC error:" << result->toString();
    }

    return ret;
}

QString JsonRpc_Client::pduMetaData(uchar addr)
{
    QString ret; if(!isConnected()) return ret;
    auto result = rpc_client->call("pduMetaData", addr);
    if (result->isSuccess()) {
        ret = result->result().toString();
    } else {
        qDebug() << Q_FUNC_INFO << "RPC error:" << result->toString();
    }

    return ret;
}

double JsonRpc_Client::pduDataGet(uchar addr,  uchar type, uchar topic, uchar sub, uchar id)
{
    double ret = -1; if(!isConnected()) return ret;
    auto result = rpc_client->call("pduDataGet", addr, type, topic, sub, id);
    if (result->isSuccess()) {
        ret = result->result().toDouble();
    } else {
        qDebug() << Q_FUNC_INFO << "RPC error:" << result->toString();
    }

    return ret;
}

bool JsonRpc_Client::pduDataSet(uchar addr,  uchar type, uchar topic, uchar sub, uchar id, double value)
{
    bool ret = false; if(!isConnected()) return ret;
    auto result = rpc_client->call("pduDataSet", addr, type, topic,sub, id, value);
    if (result->isSuccess()) {
        ret = result->result().toBool();
    } else {
        qDebug() << Q_FUNC_INFO << "RPC error:" << result->toString();
    }

    return ret;
}

QVariant JsonRpc_Client::pduCfgGet(uchar type, uchar fc, uchar id, uchar addr)
{
    QVariant ret; if(!isConnected()) return ret;
    auto result = rpc_client->call("pduCfgGet", type, fc, id, addr);
    if (result->isSuccess()) {
        ret = result->result();
    } else {
        qDebug() << Q_FUNC_INFO << "RPC error:" << result->toString();
    }

    return ret;
}

bool JsonRpc_Client::pduCfgSet(uchar type, uchar fc, const QVariant &value, uchar id, uchar addr)
{
    bool ret = false; if(!isConnected()) return ret;
    auto result = rpc_client->call("pduCfgSet", type, fc, value, id, addr);
    if (result->isSuccess()) {
        ret = result->result().toBool();
    } else {
        qDebug() << Q_FUNC_INFO << "RPC error:" << result->toString();
    }

    return ret;
}

bool JsonRpc_Client::pduRelaysCtrl(int addr, int start, int num, uchar on)
{
    bool ret = false; if(!isConnected()) return ret;
    auto result = rpc_client->call("pduRelaysCtrl", addr, start, num, on);
    if (result->isSuccess()) {
        ret = result->result().toBool();
    } else {
        qDebug() << Q_FUNC_INFO << "RPC error:" << result->toString();
    }

    return ret;
}

QString JsonRpc_Client::pduLogFun(uchar type, uchar fc, int id, int cnt)
{
    QString str; if(!isConnected()) return str;
    auto result = rpc_client->call("pduLogFun", type, fc, id, cnt);
    if (result->isSuccess()) {
        str = result->result().toString();
    } else {
        qDebug() << Q_FUNC_INFO << "RPC error:" << result->toString();
    }
    return str;
}

void JsonRpc_Client::close()
{
    if(rpc_client) rpc_client->disconnectFromServer();
}

// 处理不请自来的通知
void JsonRpc_Client::initReceiveNotification()
{
    connect(rpc_client, &jcon::JsonRpcClient::notificationReceived,
            this, [](const QString& key, const QVariant& value){
        qDebug() << "Received notification:"
                 << "Key:" << key
                 << "Value:" << value;
    });
}

// "ws://127.0.0.1:6002"
bool JsonRpc_Client::startClient(const QString &host, int port, SocketType socket_type)
{
    if(rpc_client) {close(); delete rpc_client;}
    if (socket_type == SocketType::tcp) {
        rpc_client = new jcon::JsonRpcTcpClient(this);
    } else {
        rpc_client = new jcon::JsonRpcWebSocketClient(this);
    }

    initReceiveNotification();
    bool ret = rpc_client->connectToServer(host, port);
    if(ret) rpc_client->enableReceiveNotification(true);
    return ret;
}

bool JsonRpc_Client::startLocalClient(int port)
{
    mPort = port; timer->start(3500 + rand()%100);
    bool ret = startClient("127.0.0.1", port);
    //if(!ret) qDebug() << Q_FUNC_INFO <<  "start Local Client" << port << ret;
    return ret;
}

void JsonRpc_Client::timeoutDone()
{
    if(mPort && rpc_client) {
        if(!rpc_client->isConnected()) {
            startLocalClient(mPort);
        }
    }
}

bool JsonRpc_Client::isConnected()
{
    bool ret = false;
    if(mPort && rpc_client) {
        ret = rpc_client->isConnected();
        if(!ret) ret = startLocalClient(mPort);
    }
    return ret;
}
