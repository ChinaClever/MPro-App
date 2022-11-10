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
    QtConcurrent::run(this,&Integr_PushThread::run);
    connect(this, &Integr_PushThread::pushSig, this, &Integr_PushThread::onPushSlot);
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
}

void Integr_PushThread::httpPush(const QByteArray &array)
{
    if(mCfg->http.url.isEmpty()) return ;
    switch (mCfg->http.en) {
    case 0: break;
    case 1: Http::post(mCfg->http.url, array, mCfg->http.timeout); break;
    case 2: Http::put(mCfg->http.url, array, mCfg->http.timeout); break;
    default: qDebug() << Q_FUNC_INFO; break;
    }
}

void Integr_PushThread::mqttPush(const QByteArray &array)
{
    if(Mqtt_Client::cfg.isConnected) {
        emit Mqtt_Client::bulid()->publish(array);
    }
}

void Integr_PushThread::amqpPush(const QByteArray &array)
{
    if(QRabbitMQ::amqpCfg.isConnected) {
        emit QRabbitMQ::bulid()->sendMsgSig(array);
    }
}

void Integr_PushThread::onPushSlot()
{
    udpPush(mArray);
    mqttPush(mArray);
    httpPush(mArray);
    amqpPush(mArray);
}

void Integr_PushThread::workDown()
{
    for(int i=0; i<DEV_NUM; ++i) {
        sDevData *dev = cm::devData(i);
        if(dev->offLine || i==0) {
            mArray = mJson->getJson(i);
            emit pushSig();
        } if(isRun) delay();
    }
}

void Integr_PushThread::delay()
{
    int t = QRandomGenerator::global()->bounded(10);
    int sec = mCfg->sec; if(!sec) sec = 3;
    for(int i=0; i<sec*100; i+=100) {
        if(isRun) cm::mdelay(100); else break;
    } cm::mdelay(t);
}

bool Integr_PushThread::checkPush()
{
    bool ret = false;
    ret |= QRabbitMQ::amqpCfg.isConnected;
    ret |= Mqtt_Client::cfg.isConnected;
    ret |= mCfg->udp[0].en;
    ret |= mCfg->udp[1].en;
    ret |= mCfg->http.en;

    return ret;
}

void Integr_PushThread::run()
{
    while (isRun) {
        int t = QRandomGenerator::global()->bounded(100);
        cm::mdelay(t);delay(); if(checkPush()) workDown();
    }
}
