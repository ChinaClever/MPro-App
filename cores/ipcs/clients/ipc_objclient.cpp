/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ipc_objclient.h"

IPC_ObjClient::IPC_ObjClient(QObject *parent)
    : QObject{parent}
{
    mSocket = new Domain_SocketCli(this);
}

sDataPacket *IPC_ObjClient::dataPacket()
{
    sDataPacket *res = (sDataPacket *)Shm::sharedMemory();
    return res;
}

bool IPC_ObjClient::sendSocket(const QVariantList &v)
{
    QByteArray array; QDataStream in(&array, QIODevice::WriteOnly);
    in << v.first().toInt() << v.last().toByteArray();
    int ret = mSocket->send(array);
    return ret>0 ? true: false;
}

QVariant IPC_ObjClient::readSocket(const QVariantList &v, int msec)
{
    QByteArray array; QDataStream in(&array, QIODevice::WriteOnly);
    in << v.first().toInt() << v.last().toByteArray();
    return mSocket->trans(array, msec);
}

