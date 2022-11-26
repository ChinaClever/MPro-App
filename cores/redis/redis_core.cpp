/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "redis_core.h"
#include "commons.h"

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

void Redis_Core::workDown()
{
    qDebug() << set("lzy", "luozhiyong", "123456789");
    qDebug() << expipe("lzy", redisCfg.alive);
    //cout << "AAAAAAAAA";
}

void Redis_Core::run()
{
    while(isRun) {
        int sec = redisCfg.sec;
        cm::mdelay(sec*1000+1);
        if(redisCfg.en) {
            connectServer();
            workDown();
        }
    }
}
