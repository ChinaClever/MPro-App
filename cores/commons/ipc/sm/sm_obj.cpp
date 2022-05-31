/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "sm_obj.h"

#define LINUX_SHM 1

#if LINUX_SHM
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define SHM_KEY	0x5105
static void *share_mem_get(int size)
{
    static void *shm = nullptr; if(shm) return shm;
    key_t key = ftok("/tmp", SHM_KEY);
    int shmid = shmget(key, size, 0666|IPC_CREAT);    //创建共享内存
    if(shmid == -1) {
        fprintf(stderr, "shmget failed\n");
        return NULL;
    }

    shm = shmat(shmid, 0, 0); //将共享内存连接到当前进程的地址空间
    if(shm == (void*)-1) {
        fprintf(stderr, "shmat failed\n");
        return NULL;
    } //else memset(shm, 0, size);

    return shm;
}
#endif

QSharedMemory* SM_Obj::mSm = nullptr;
SM_Obj::SM_Obj(QObject *parent) : QObject{parent}
{
#if LINUX_SHM

#else
    initShareMemory(parent);
#endif
}

void SM_Obj::initShm()
{
#if LINUX_SHM
    int size = sizeof(sDataPacket);
    void *shm = sharedMemory();
    memset(shm, 0, size);
#endif
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
#if LINUX_SHM
     return share_mem_get(sizeof(sDataPacket));
#else
    if(!mSm) initShareMemory();
    return mSm->data();
#endif
}
