/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ipc_outputserver.h"

IPC_OutputServer::IPC_OutputServer(QObject *parent)
    : IPC_ObjServer{parent}
{

}

IPC_OutputServer *IPC_OutputServer::bulid(QObject *parent)
{
    static IPC_OutputServer *sington = nullptr;
    if(!sington) {
        sington = new IPC_OutputServer(parent);
        sington->initFunction(IPC_KEY_RELAY);
    }
    return sington;
}

QList<const char *> IPC_OutputServer::busRecvMethods()
{
    QList<const char *> res;
    res << SLOT(dbus_recv_slot(int, int, int, QString));
    return res;
}

void IPC_OutputServer::dbus_recv_slot(int addr, int id, int fc, const QString &msg)
{
    Set_Core *set = Set_Core::bulid();
    switch (fc) {
    case 1: set->outputRelayCtrl(addr, id, msg.toInt()); break;
    case 2: set->outputDelaySet(addr, id, msg.toInt()); break;
    case 3: set->outputSwModeSet(addr, id, msg.toInt()); break;
    case 4: set->outputNameSet(addr, id, msg); break;
    default: qDebug() << Q_FUNC_INFO << addr << id << fc << msg; break;
    }
}


