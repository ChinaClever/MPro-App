#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

#define SHM_KEY	0x5105
#define SUPPORT_C 1
#include "/home/lzy/work/NPDU/cores/commons/cms/datapacket.h"
#include <QtCore>

static const QDateTime buildDateTime()
{
   QString dateTime;
   dateTime += __DATE__; dateTime += __TIME__;
   dateTime.replace("  "," 0");//注意" "是两个空格，用于日期为单数时需要转成“空格+0”
   return QLocale(QLocale::English).toDateTime(dateTime, "MMM dd yyyyhh:mm:ss");
}

static void compileTime(sDataPacket *shm)
{
    sVersions *vers = &(shm->data[0].cfg.vers);
    QString str = buildDateTime().toString("yyyy-MM-dd hh:mm:ss");
    qstrcpy(vers->startCompileTime, str.toLatin1().data());
}

void init_share_mem()
{
    static struct sDataPacket *shm = NULL; if(shm) return;
    int shmid = shmget((key_t)SHM_KEY, 1.1*sizeof(struct sDataPacket), 0666|IPC_CREAT);    //创建共享内存
    if(shmid >= 0) {
        shm = (sDataPacket *)shmat(shmid, 0, 0);  //将共享内存连接到当前进程的地址空间
        if(shm) memset(shm, 0, sizeof(struct sDataPacket));
        if(shm) compileTime(shm);
    } else fprintf(stderr, "shmget failed %d\n", shmid);
}
