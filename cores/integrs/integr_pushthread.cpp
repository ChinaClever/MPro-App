/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "integr_pushthread.h"
#include "http/http.h"

Integr_PushThread::Integr_PushThread(QObject *parent)
    : QObject{parent}
{
    mUdp = new Net_Udp(this);
    mJson = new Integr_JsonBuild();
    mThread = new CThread(this);
}

Integr_PushThread::~Integr_PushThread()
{
    isRun = false;
    mThread->stop();
    //wait();
}


void Integr_PushThread::push_stop(int id)
{
    switch (id) {
    case 1: mItem.udp_ip[0].clear(); break;
    case 2: mItem.udp_ip[1].clear(); break;
    case 3: mItem.http_url.clear(); break;
    default: isRun = false; break;
    }
}

void Integr_PushThread::startSlot()
{
    if(!isRun) {
        mThread->init(this, SLOT(run()));
         mThread->start();
    }
}

void Integr_PushThread::push_startHttp(const QString &url, int timeout)
{
    mItem.http_url = url;
    mItem.http_timeout = timeout;
    if(!isRun) QTimer::singleShot(50,this,SLOT(startSlot()));
}

void Integr_PushThread::push_startUdp(int id, const QString &ip, int port)
{
    if(id < INTEGR_UDP_SIZE) {
        mItem.udp_ip[id] = ip;
        mItem.port[id] = port;
        if(!isRun) QTimer::singleShot(50,this,SLOT(startSlot()));
    }
}

void Integr_PushThread::udpPush(const QByteArray &array)
{
    for(int i=0; i<INTEGR_UDP_SIZE; ++i)  {
        QString ip = mItem.udp_ip[i];
        if(ip.size()) {
            QHostAddress host(ip);
            mUdp->writeDatagram(array, host, mItem.port[i]);
        }
    }
}

void Integr_PushThread::httpPost(const QByteArray &array)
{
    QUrl url(mItem.http_url); //https://github.com/flaviotordini/http
    Http::instance().setReadTimeout(mItem.http_timeout);
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
        if(dev->offLine || i==0) {
            QByteArray res = mJson->getJson(i);
            if(mItem.http_url.size()) httpPost(res);
            udpPush(res);
        } delay();
    }
}

void Integr_PushThread::delay()
{
    int sec = mItem.sec; if(!sec) sec = 5;
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
