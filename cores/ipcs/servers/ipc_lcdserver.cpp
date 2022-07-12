#include "ipc_lcdserver.h"

IPC_LcdServer::IPC_LcdServer(QObject *parent)
    : Domain_SocketServ{UNIXLCD_SERV_PATH, parent}
{

}

IPC_LcdServer *IPC_LcdServer::bulid(QObject *parent)
{
    static IPC_LcdServer *sington = nullptr;
    if(!sington) {
        sington = new IPC_LcdServer(parent);
    }
    return sington;
}

QVariant IPC_LcdServer::ipc_reply(const QByteArray &array)
{
    qDebug() << "ipc_reply" << array;
    QVariant res;
    return res;
//    int fc; QByteArray msg(array), rcv;
//    QDataStream out(&msg, QIODevice::ReadOnly);
//    out >> fc >> rcv;

//    return ipc_recv_msg(fc, rcv);
}
