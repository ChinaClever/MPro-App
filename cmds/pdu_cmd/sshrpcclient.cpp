/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "sshrpcclient.h"

SshRpcClient::SshRpcClient(QObject *parent)
    : JsonRpcClient{parent}
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
