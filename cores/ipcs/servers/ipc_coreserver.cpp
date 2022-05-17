/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
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
    IPC_OutputServer::bulid(this);
    IPC_AlarmServer::bulid(this);
    IPC_CfgServer::bulid(this);
    IPC_LogServer::bulid(this);
}
