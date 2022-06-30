/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "integr_pushthread.h"
#include "http/http.h"

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

void Integr_PushThread::startFun(const sPushIt &it)
{
    mItem = it;
    if(!isRun) start();
}

void Integr_PushThread::udpPush(const QByteArray &array)
{
    QHostAddress host(mItem.ip);
    mUdp->writeDatagram(array, host, mItem.port);
}

void Integr_PushThread::httpPost(const QByteArray &array)
{
    QUrl url(mItem.url); //https://github.com/flaviotordini/http
    Http::instance().setReadTimeout(mItem.timeout);
    auto reply = Http::instance().post(url, array, "application/json");
    connect(reply, &HttpReply::finished, this, [](auto &reply) {
        if (reply.isSuccessful()) {
            qDebug() << "Feel the bytes!" << reply.body();
        } else {
            qDebug() << "Something's wrong here" << reply.statusCode() << reply.reasonPhrase();
        }
    });
}

void Integr_PushThread::workDown()
{
    for(int i=0; i<DEV_NUM; ++i) {
        sDevData *dev = cm::devData(i);
        if(dev->offLine) {
            QByteArray res = mJson->getJson(i);
            switch (mItem.type) {
            case 1: udpPush(res);break;
            case 2: httpPost(res);break;
            }
        } delay();
    }
}

void Integr_PushThread::delay()
{
    int sec = mItem.sec; if(!sec) sec = 5;
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
