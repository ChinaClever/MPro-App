#include "ipc_objserver.h"

IPC_ObjServer::IPC_ObjServer(QObject *parent) : LSocket_Server{parent}
{

}

sDataPacket *IPC_ObjServer::dataPacket()
{
    return (sDataPacket *)sharedMemory();
}

void IPC_ObjServer::initFunction(const QString &key, bool f)
{
    setKey(key); if(f)lsListen();
    registerBusObject();
    busConnects();
}
