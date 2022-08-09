/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "integr_pushthread.h"
#include "http.h"

sPushCfg Integr_PushThread::pushCfg;
Integr_PushThread::Integr_PushThread(QObject *parent)
    : QObject{parent}
{
    mUdp = new Net_Udp(this);
    mJson = Integr_JsonBuild::bulid();
    mThread = new CThread(this);
}

Integr_PushThread::~Integr_PushThread()
{
    isRun = false;
    mThread->stop();
    //wait();
}

void Integr_PushThread::startSlot()
{
    if(!isRun) {
        // QtConcurrent::run(this, &Integr_PushThread::run);
        mThread->init(this, SLOT(run()));
        mThread->start();
    }
}

void Integr_PushThread::udpPush(const QByteArray &array)
{
    for(int i=0; i<INTEGR_UDP_SIZE; ++i)  {
        QString ip =mCfg->udp[i].host;
        if(mCfg->udp[i].en && ip.size()) {
            QHostAddress host(ip);
            mUdp->writeDatagram(array, host, mCfg->udp[i].port);
        }
    }
//    QHostAddress host("192.168.1.100");
//    mUdp->writeDatagram(array, host, 8766);
}

void Integr_PushThread::httpPush(const QByteArray &array)
{
    if(mCfg->http.url.isEmpty()) return ;
    switch (mCfg->http.en) {
    case 1: Http::post(mCfg->http.url, array, mCfg->http.timeout); break;
    case 2: Http::put(mCfg->http.url, array, mCfg->http.timeout); break;
    default: qDebug() << Q_FUNC_INFO; break;
    }
}

void Integr_PushThread::workDown()
{
    for(int i=0; i<DEV_NUM; ++i) {
        sDevData *dev = cm::devData(i);
        if(dev->offLine || i==0) {
            QByteArray res = mJson->getJson(i);
            udpPush(res); httpPush(res);
        } if(isRun) delay();
    }
}

void Integr_PushThread::delay()
{
    int sec = mCfg->sec; if(!sec) sec = 3;
    for(int i=0; i<sec*100; i+=100) {
        if(isRun) cm::mdelay(100); else break;
    } int t = QRandomGenerator::global()->bounded(100); cm::mdelay(t);
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
