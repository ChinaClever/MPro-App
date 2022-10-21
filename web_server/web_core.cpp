/*
 *
 *  Created on: 2022年10月1日
 *      Author: Pmd
 */
#include "web_core.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

Web_Core::Web_Core(QObject *parent) : Web_Http{parent}
{
    QTimer::singleShot(150,this,SLOT(initFunSlot()));
}

Web_Core *Web_Core::bulid(QObject *parent)
{
    static Web_Core* sington = nullptr;
    if(sington == nullptr) sington = new Web_Core(parent);
    return sington;
}

void Web_Core::initFunSlot()
{
    init_share_mem(); if(mRun) web_initFun();
    timer = new QTimer(this); timer->start(1000);
    connect(timer, SIGNAL(timeout()),this, SLOT(web_onTimeoutDone()));
}

void Web_Core::init_webCfg(sWebCfg &web)
{
    cfg.http_en = web.http_en;
    cfg.http_port = web.http_port;
    cfg.http_redirect = web.http_redirect;

    cfg.https_en = web.https_en;
    cfg.https_port = web.https_port;
}

void Web_Core::init_share_mem()
{
    static struct sDataPacket *shm = NULL; if(shm) return;
    key_t key = ftok("/tmp", SHM_KEY);
    int shmid = shmget(key, sizeof(struct sDataPacket), 0666|IPC_CREAT);    //创建共享内存
    if(shmid >= 0) {
        shm = (sDataPacket *)shmat(shmid, 0, 0);  //将共享内存连接到当前进程的地址空间
        if(shm) {mRun = &(shm->data->proc.web); init_webCfg(shm->web);}
    } else fprintf(stderr, "shmget failed %d\n", shmid);
}

void Web_Core::web_initFun()
{
    QString t = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    qstrcpy(mRun->start, t.toLatin1().data());

    QString dateTime;
    dateTime += __DATE__; dateTime += __TIME__;
    dateTime.replace("  "," 0");//注意" "是两个空格，用于日期为单数时需要转成“空格+0”
    QDateTime dt = QLocale(QLocale::English).toDateTime(dateTime, "MMM dd yyyyhh:mm:ss");
    QString str = dt.toString("yyyy-MM-dd hh:mm:ss");
    qstrcpy(mRun->compileTime, str.toUtf8().data());
}

void Web_Core::web_onTimeoutDone()
{
    if(mRun)mRun->runSec += 1;
}
