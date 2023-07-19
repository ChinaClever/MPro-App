/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "redis_core.h"
#include "commons.h"
#include "integr_core.h"

Redis_Core::Redis_Core(QObject *parent)
    : Redis_Obj{parent}
{
    QtConcurrent::run(this,&Redis_Core::run);
}

Redis_Core *Redis_Core::bulid(QObject *parent)
{
    static Redis_Core* sington = NULL;
    if(sington == NULL) {
        sington = new Redis_Core(parent);
    }
    return sington;
}

void Redis_Core::redisHandleMessage(const QStringList &msg)
{
    QString ip = cm::dataPacket()->net.inet.ip;
    if(msg.at(1) == redisCfg.subscribe || msg.at(1) == ip) {
        Integr_JsonRecv::bulid()->recv(msg.last().toUtf8());
    } else qDebug() << "error redisHandleMessage" << msg;
}

void Redis_Core::workDown()
{
    sRedisCfg *cfg = &redisCfg;
    int size = cm::masterDev()->cfg.nums.slaveNum;
    for(int i=0; i<=size; ++i) {
        QString key = cm::devData(i)->cfg.uut.uuid;
        QByteArray array = Integr_JsonBuild::bulid()->getJson(i);
        if(cfg->key.size()) key = cfg->key +":" + QString::number(i);
        bool ret = set(key, "pduMetaData", array); if(ret) expipe(key, cfg->alive);
    }
}

void Redis_Core::run()
{
    while(isRun) {        
        int t = QRandomGenerator::global()->bounded(100);
        int sec = redisCfg.sec; cm::mdelay(sec*1000+t);
        if(redisCfg.en) {
            connectServer(); QString sub = redisCfg.subscribe;
            if(sub.isEmpty()) sub = cm::dataPacket()->net.inet.ip;
            subscribe(sub); workDown(); disconnect();
        }
    }
}
