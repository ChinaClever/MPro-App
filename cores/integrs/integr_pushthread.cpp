/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "integr_pushthread.h"

Integr_PushThread::Integr_PushThread(QObject *parent)
    : QThread{parent}
{
    mJson = new Integr_JsonBuild();
    mUdp = new Net_Udp(this);
}

Integr_PushThread::~Integr_PushThread()
{
    isRun = false;
    wait();
}

Integr_PushThread *Integr_PushThread::bulid(QObject *parent)
{
    static Integr_PushThread* sington = NULL;
    if(sington == NULL) {
        sington = new Integr_PushThread(parent);
    }
    return sington;
}


void Integr_PushThread::stopFun()
{
    isRun = false;
}

void Integr_PushThread::startFun()
{
    if(!isRun) start();
}

void Integr_PushThread::workDown()
{
    for(int i=0; i<DEV_NUM; ++i) {
        sDevData *dev = cm::devData(i);
        if(dev->offLine) {
            QHostAddress host(mHost);
            QByteArray res = mJson->getJson(i);
            mUdp->writeDatagram(res, host, mPort);
        } delay();
    }
}

void Integr_PushThread::delay()
{
    int sec = mSec; if(!sec) sec = 5;
    for(int i=0; i<sec*100; i+=100) {
        if(isRun) msleep(100); else break;
    } int t = QRandomGenerator::global()->bounded(100); msleep(t);
}

void Integr_PushThread::run()
{
    if(isRun) return;
    else isRun = true;
    while (isRun) {
        delay();
        workDown();
    }
}
