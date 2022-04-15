#include "ipc_relayserver.h"

IPC_RelayServer::IPC_RelayServer(QObject *parent)
    : IPC_ObjServer{parent}
{

}

IPC_RelayServer *IPC_RelayServer::bulid(QObject *parent)
{
    static IPC_RelayServer *sington = nullptr;
    if(!sington) {
        sington = new IPC_RelayServer(parent);
        sington->initFunction(IPC_KEY_RELAY);
    }
    return sington;
}

QList<const char *> IPC_RelayServer::busRecvMethods()
{
    QList<const char *> res;
    res << SLOT(dbus_recv_slot(int, int, int));
    return res;
}

void IPC_RelayServer::dbus_recv_slot(int addr, int id, int value)
{
    Set_Core *set = Set_Core::bulid();
    set->outputRelayCtrl(addr, id, value);
    qDebug() << "GGGG" << addr << id << value;
}


