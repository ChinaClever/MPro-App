/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ipc_webserver.h"
#include "log_core.h"

IPC_WebServer::IPC_WebServer(QObject *parent)
    : IPC_ObjServer{parent}
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
            bool ret = Set_Core::bulid()->upMetaData(unit);
            if(ret) res = QString::number(unit.value);
        }
    } else if(5 == fc) {
        sCfgItem unit = cm::toStruct<sCfgItem>(array);
        res = Set_Core::bulid()->getCfg(unit);
    } else if(6 == fc) {
        QByteArray msg(array), rcv; QVariant value;
        QDataStream out(&msg, QIODevice::ReadOnly);
        out >> rcv >> value;
        sCfgItem it = cm::toStruct<sCfgItem>(rcv);
        Set_Core::bulid()->setCfg(it, value);
    }else if(11 == fc) {
        sLogFcIt it = cm::toStruct<sLogFcIt>(array);
        res = Log_Core::bulid()->log_readFun(it);
    }

    return res;
}

