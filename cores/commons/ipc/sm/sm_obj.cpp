/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "sm_obj.h"

#if defined(Q_OS_LINUX)
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define SHM_KEY	0x5105
static void *share_mem_get(int size)
{
    static void *shm = nullptr; if(shm) return shm;
    int shmid = shmget((key_t)SHM_KEY, size, 0666|IPC_CREAT);    //创建共享内存
    if(shmid == -1) {
        fprintf(stderr, "shmget failed\n");
        return NULL;
    }

    shm = shmat(shmid, 0, 0); //将共享内存连接到当前进程的地址空间
    if(shm == (void*)-1) {
        fprintf(stderr, "shmat failed\n");
        return NULL;
    } else memset(shm, 0, size);

    return shm;
}
#endif

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
        int size = sizeof(sDataPacket);
        bool ret = mSm->create(size);
        if(!mSm->isAttached()) mSm->attach();
        if(ret) memset(mSm->data(), 0, size);
    }
}

void *SM_Obj::sharedMemory()
{
#if defined(Q_OS_LINUX)
     return share_mem_get(sizeof(sDataPacket));
#else
    if(!mSm) initShareMemory();
    return mSm->data();
#endif
}
