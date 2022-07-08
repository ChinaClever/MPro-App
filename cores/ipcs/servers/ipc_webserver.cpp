/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ipc_webserver.h"

IPC_WebServer::IPC_WebServer(QObject *parent)
    : IPC_LogServer{parent}
{

}

IPC_WebServer *IPC_WebServer::bulid(QObject *parent)
{
    static IPC_WebServer *sington = nullptr;
    if(!sington) {
        sington = new IPC_WebServer(parent);
    }
    return sington;
}

QVariant IPC_WebServer::ipc_recv_msg(int fc, const QByteArray &array)
{
    QVariant res;
    if(1 == fc) {
        sDataItem unit = cm::toStruct<sDataItem>(array);
        if(unit.rw) Set_Core::bulid()->setting(unit);
        else {
            bool ret = Set_Core::bulid()->upIndexValue(unit);
            if(ret) res = QString::number(unit.value);
        }
    } else if(2 == fc) {
        sNumStrItem unit = cm::toStruct<sNumStrItem>(array);
        if(unit.rw) Set_Core::bulid()->setNumStr(unit);
        else res = Set_Core::bulid()->getNumStr(unit);
    } else if(6 == fc) {
        sIpcLog it = cm::toStruct<sIpcLog>(array);
        res = logFun(it);
    }

    return res;
}

