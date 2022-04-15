#include "ipc_delayserver.h"

IPC_DelayServer::IPC_DelayServer(QObject *parent)
    : IPC_ObjServer{parent}
{

}

IPC_DelayServer *IPC_DelayServer::bulid(QObject *parent)
{
    static IPC_DelayServer *sington = nullptr;
    if(!sington) {
        sington = new IPC_DelayServer(parent);
        sington->initFunction(IPC_KEY_DELAY);
    }
    return sington;
}

QList<const char *> IPC_DelayServer::busRecvMethods()
{
    QList<const char *> res;
    res << SLOT(dbus_recv_slot(int, int, int, int));
    return res;
}

void IPC_DelayServer::dbus_recv_slot(int addr, int id, int mode, int delay)
{
    Set_Core *set = Set_Core::bulid();
    set->outputDelaySet(addr, id, mode, delay);
}
