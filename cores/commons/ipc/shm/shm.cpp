/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "shm.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define SHM_KEY	0x5105

static void *share_mem_get(uint size)
{
    static void *shm = nullptr; if(shm) return shm;
    key_t key = ftok("/tmp", SHM_KEY);
    int shmid = shmget(key, size, 0666|IPC_CREAT);    //创建共享内存
    if(shmid == -1) {
        fprintf(stderr, "share mem shmget failed\n");
        return NULL;
    }

    shm = shmat(shmid, 0, 0); //将共享内存连接到当前进程的地址空间
    if(shm == (void*)-1) {
        fprintf(stderr, "share mem shmat failed\n");
        return NULL;
    } //else memset(shm, 0, size);

    return shm;
}


void Shm::initShm()
{
    uint size = sizeof(sDataPacket);
    void *shm = share_mem_get(size);
    if(shm) memset(shm, 0, size);
}

void *Shm::sharedMemory()
{
    return share_mem_get(sizeof(sDataPacket));
}
