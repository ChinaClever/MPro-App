/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "sshrpcclient.h"

SshRpcClient::SshRpcClient(QObject *parent)
    : JsonRpc_Client{parent}
{
    bool ret = startLocalClient(9224);
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
