#include "sm_obj.h"
QSharedMemory* SM_Obj::mSm = nullptr;
SM_Obj::SM_Obj(QObject *parent) : QObject{parent}
{
    initShareMemory(parent);
}

void SM_Obj::initShareMemory(QObject *parent)
{
    if(!mSm) {
        mSm = new QSharedMemory(parent);
        mSm->setKey(IPC_SHAREDMEMORY_KEY);
        sharedMemory(sizeof(sDataPacket));
    }
}

void *SM_Obj::sharedMemory(int size)
{
    initShareMemory();
    bool ret = mSm->create(size);
    if(!mSm->isAttached()) mSm->attach();
    if(ret) memset(mSm->data(), 0, size*1.1);
    return mSm->data();
}

void *SM_Obj::sharedMemory()
{
    if(!mSm) initShareMemory();
    return mSm->data();
}
