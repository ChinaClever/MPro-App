/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ipc_objserver.h"

IPC_ObjServer::IPC_ObjServer(QObject *parent) : Domain_SocketServ{UNIXDG_SERV_PATH, parent}
{

}

QVariant IPC_ObjServer::ipc_reply(const QByteArray &array)
{
    int fc; QByteArray msg(array), rcv;
    QDataStream out(&msg, QIODevice::ReadOnly);
    out >> fc >> rcv;

    return ipc_recv_msg(fc, rcv);
}
