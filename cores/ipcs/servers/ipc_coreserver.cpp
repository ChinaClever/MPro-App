#include "ipc_coreserver.h"

IPC_CoreServer::IPC_CoreServer(QObject *parent)
    : QObject{parent}
{
    QTimer::singleShot(45,this,SLOT(initFunSlot()));
}


IPC_CoreServer *IPC_CoreServer::bulid(QObject *parent)
{
    static IPC_CoreServer *sington = nullptr;
    if(!sington) {
        sington = new IPC_CoreServer(parent);
        DBus_Object::registerBusService();
    }
    return sington;
}

void IPC_CoreServer::initFunSlot()
{
    IPC_DelayServer::bulid(this);
    IPC_RelayServer::bulid(this);
    IPC_AlarmServer::bulid(this);
}
