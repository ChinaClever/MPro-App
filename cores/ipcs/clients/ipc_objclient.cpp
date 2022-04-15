#include "ipc_objclient.h"

IPC_ObjClient::IPC_ObjClient(QObject *parent)
    : QObject{parent}
{
    mDbus = nullptr;
}

sDataPacket *IPC_ObjClient::dataPacket()
{
    sDataPacket *res = nullptr;
    if(mDbus) res = (sDataPacket *)mDbus->sharedMemory();
    return res;
}

void IPC_ObjClient::initFunction(const QString &key, bool f)
{
    if(!mDbus) mDbus = new DBus_Call(key, this);
    if(f) QTimer::singleShot(1, mDbus, SLOT(lscReconnect()));
}

QVariant IPC_ObjClient::readBus(const QVariantList &v)
{
    QVariant res;
    if(inputCheck(v)) {
        QVariantList lv = mDbus->callBus(v);
        if(lv.size()) res = lv.first();
    }
    return  res;
}

