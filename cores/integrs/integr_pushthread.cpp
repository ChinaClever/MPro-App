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
        sPushUdpCfg *cfg = &mCfg->udp[i]; if(!cfg->sec) return;
        if(cfg->en && cfg->host.size() && (0 == (mCnt%cfg->sec))) {
            QHostAddress host(cfg->host); //cout << "udp" << ip << mCfg->udp[i].port;
            mUdp->writeDatagram(array, host, cfg->port);
        }
    }
}

void Integr_PushThread::httpPush(const QByteArray &array)
{
    sPushHttpCfg *cfg = &mCfg->http; if(!cfg->sec) return;
    if(cfg->url.isEmpty() || (mCnt%cfg->sec)) return ;
    switch (cfg->en) { case 0: break;
    case 1: Http::post(cfg->url, array, cfg->timeout); break;
    case 2: Http::put(cfg->url, array, cfg->timeout); break;
    default: cout << cfg->url; break;
    }
}

void Integr_PushThread::mqttPush(const QByteArray &array)
{
    sMqttCfg *cfg = &Mqtt_Client::cfg; if(!cfg->sec) return;
    if(cfg->isConnected && (0 == (mCnt%cfg->sec))) {
        emit Mqtt_Client::bulid()->publish(mAddr, array);
    }
}

void Integr_PushThread::amqpPush(const QByteArray &array)
{
    sAmqpCfg *cfg = &QRabbitMQ::amqpCfg; if(!cfg->sec) return;
    if(cfg->isConnected && (0 == (mCnt%cfg->sec))) {
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
            mArray = mJson->getJson(i); //cout << mArray.size();
            if(cm::masterDev()->cfg.param.oldProtocol) { ///////======
                //mArray = cm::zipCompress(mArray); //cout << mArray.size();
            } mAddr = i; emit pushSig();
        } if(isRun) delay(20);
    }
}

void Integr_PushThread::delay(int msec)
{
    int v = 10; if(msec > 100) v = msec/10;
    int t = QRandomGenerator::global()->bounded(v);
    cm::mdelay(t + msec);
}

bool Integr_PushThread::checkPush()
{
    bool ret = false; mCnt += 1;  delay(1000);
    for(int i=0; i<INTEGR_UDP_SIZE; ++i) ret |= mCfg->udp[i].en;
    ret |= QRabbitMQ::amqpCfg.isConnected;
    ret |= Mqtt_Client::cfg.isConnected;
    ret |= mCfg->http.en;
    return ret;
}

void Integr_PushThread::run()
{
    while (isRun) {
        if(checkPush()) workDown();
    }
}
