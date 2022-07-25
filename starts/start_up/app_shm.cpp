#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

#define SHM_KEY	0x5105
#define SUPPORT_C 1
#include "/home/lzy/work/NPDU/cores/commons/cms/datapacket.h"

void init_share_mem()
{
    static struct sDataPacket *shm = NULL; if(shm) return;
    int shmid = shmget((key_t)SHM_KEY, sizeof(struct sDataPacket), 0666|IPC_CREAT);    //创建共享内存
    if(shmid  > 0) shm = (sDataPacket *)shmat(shmid, 0, 0);  //将共享内存连接到当前进程的地址空间
    else fprintf(stderr, "shmget failed\n");

    size_t size = sizeof(struct sDataPacket);
    if(shm) memset(shm, 0, size);
}
