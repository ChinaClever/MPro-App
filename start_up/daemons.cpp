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
    init_share_mem();
    if(mProcs) start_proc();
}

Daemons *Daemons::bulid()
{
    static Daemons* sington = nullptr;
    if(!sington) sington = new Daemons();
    return sington;
}

void Daemons::start_proc()
{     
    initFun(); //factoryRestore();
    proc_start(mProcs->core, "cores");
    proc_start(mProcs->awtk, "awtk");
    proc_start(mProcs->web, "web_server");
    proc_start(mProcs->ota, "ota_updater");
    QtConcurrent::run(this,&Daemons::workDown);
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

void Daemons::initFun()
{
    QString t = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    sRunTime *run = &(mProcs->daemon);
    qstrcpy(run->start, t.toLatin1().data());

    QString dateTime; proc_log("daemon_startup");
    dateTime += __DATE__; dateTime += __TIME__;
    dateTime.replace("  "," 0");//注意" "是两个空格，用于日期为单数时需要转成“空格+0”
    QDateTime dt = QLocale(QLocale::English).toDateTime(dateTime, "MMM dd yyyyhh:mm:ss");
    QString str = dt.toString("yyyy-MM-dd hh:mm:ss");
    qstrcpy(run->compileTime, str.toUtf8().data());    
}

void Daemons::resetProc(sRunTime &proc, const QString &name)
{
    QString cmd = "killall " + name;
    system("chmod 755 -R /usr/data/clever/bin");
    system("chmod 755 -R /usr/data/clever/app");
    system(cmd.toLatin1().data()); mdelay(100);
    proc_log(name +"_exit"); mdelay(100);
    proc_start(proc, name); mdelay(5000);
}


bool Daemons::procRunStatus(sRunTime &proc, const QString &name)
{
    bool ret = true;
    if(proc.runSec > proc.daemonSec) {
        proc.daemonSec = proc.runSec;
    } else if(!proc_isRun(name)){
        resetProc(proc, name);
        ret = false; qDebug() << __FUNCTION__ << name;
    } mdelay(100);
    return ret;
}

void Daemons::workDown()
{
    mdelay(6500); while(1) {
        procRunStatus(mProcs->core, "cores");
        procRunStatus(mProcs->web, "web_server");
        procRunStatus(mProcs->ota, "ota_updater");
        mProcs->daemon.runSec += 1; mdelay(1800);
        if(5 == mProcs->daemon.runSec%30) {
            procRunStatus(mProcs->awtk, "awtk");
        }
    }
}
