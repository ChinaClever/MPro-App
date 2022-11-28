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
    if(msg.at(1) == redisCfg.subscribe) {
        Integr_JsonRecv::bulid()->recv(msg.last().toUtf8());
    } else qDebug() << "error redisHandleMessage" << msg;
}

void Redis_Core::workDown()
{
    sRedisCfg *cfg = &redisCfg;
    QMap<QByteArray, QVariant> map;
    int size = cm::masterDev()->cfg.nums.slaveNum;
    for(int i=0; i<=size; ++i) {
        QByteArray array = Integr_JsonBuild::bulid()->getJson(i);
        map[QByteArray::number(i)] = array;
    }
    bool ret = set(cfg->key, map);
    if(ret) expipe(cfg->key, cfg->alive);
}

void Redis_Core::run()
{
    while(isRun) {
        int sec = redisCfg.sec;
        cm::mdelay(sec*1000+1);
        if(redisCfg.en && redisCfg.subscribe.size()) {
            connectServer();
            subscribe();
            workDown();
        } else disconnect();
    }
}
