/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ipc_webclient.h"

IPC_WebClient::IPC_WebClient(QObject *parent)
    : IPC_OutputClient{parent}
{

}

IPC_WebClient *IPC_WebClient::bulid(QObject *parent)
{
    static IPC_WebClient *sington = nullptr;
    if(!sington) {
        sington = new IPC_WebClient(parent);
    }
    return sington;
}

bool IPC_WebClient::devNameSet(int addr, const QString &value, int id)
{
     return setString(addr, SFnCode::Uuts, id, value);
}

bool IPC_WebClient::upload(const QString &fn, const QString &md5)
{
    return multipleStrings(2, fn, md5);
}
