/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "daemons.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <QtConcurrent>

Daemons::Daemons()
{
    mProcs = nullptr;
    init_share_mem(); start_proc();
    QtConcurrent::run(this,&Daemons::workDown);
}


Daemons *Daemons::bulid()
{
    static Daemons* sington = nullptr;
    if(!sington) sington = new Daemons();
    return sington;
}

void Daemons::start_proc()
{
    cm_mdelay(152);
    startCore();
    //startLcd();   //////////==========

}

void Daemons::init_share_mem()
{
    static struct sDataPacket *shm = NULL; if(shm) return;
    key_t key = ftok("/tmp", SHM_KEY);
    int shmid = shmget(key, sizeof(struct sDataPacket), 0666|IPC_CREAT);    //创建共享内存
    if(shmid >= 0) {
        shm = (sDataPacket *)shmat(shmid, 0, 0);  //将共享内存连接到当前进程的地址空间
        if(shm) memset(shm, 0, sizeof(struct sDataPacket));
        if(shm) mProcs = &(shm->data->proc);
    } else fprintf(stderr, "shmget failed %d\n", shmid);
}

bool Daemons::procRunStatus(sRunTime *proc)
{
    bool ret = true;
    if(proc->runSec > proc->daemonSec) {
        proc->daemonSec = proc->runSec;
    } else {
        ret = false;
    }
    return ret;
}

void Daemons::coreRunStatus()
{
    int t = 5000;
    sRunTime *proc = &mProcs->core;
    bool ret = procRunStatus(proc);
    if(!ret) {
        ret = isRun("cores");
        if(ret) system("killall cores");
        proc_log("core_exit");
        proc->resetCnt++;
        startCore();
    } else t = 100;
    cm_mdelay(t);
}

void Daemons::lcdRunStatus()
{
    int t = 5000;
    sRunTime *proc = &mProcs->lcd;
    bool ret = procRunStatus(proc);
    if(!ret) {
        ret = isRun("demo");
        if(ret) system("killall demo");
        proc_log("awtk_exit");
        proc->resetCnt++;
        startLcd();
    } else t = 100;
    cm_mdelay(t);
}

void Daemons::workDown()
{
    cm_mdelay(5400);
    while(1) {
        cm_mdelay(1400);
        coreRunStatus();
        //lcdRunStatus(); //////////==========

    }
}
