/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "aiot_core.h"

Aiot_Core::Aiot_Core()
{
    mCfg = &Aiot_LinkSdk::aiotCfg;
    QtConcurrent::run(this,&Aiot_Core::run);
}


Aiot_Core *Aiot_Core::bulid()
{
    static Aiot_Core* sington = nullptr;
    if(sington == nullptr) {
        sington = new Aiot_Core();
    }
    return sington;
}

void Aiot_Core::workDown()
{
    int t = mCfg->time *1000;
    if(*mCfg->connect && t) {
        basePost(); devCfgPost();
        linePost(); totalPost();
        loopPost(); outletPost();
        //eventPost("alarmLog", "EventList", "罗志勇");        

        postProperty();
        cm::mdelay(t);
        clearJson();
    } cm::mdelay(500);
}

void Aiot_Core::setting()
{
    sdkDeinit();
    *mCfg->connect = 0;
    if(mCfg->en) startSdk();
}

void Aiot_Core::run()
{
    while(isRun) {
        cm::mdelay(3500); if(mCfg->en) {
            bool ret = startSdk(); //cout << ret;
            while(ret) workDown();
            sdkDeinit();
        }
    }
}

bool Aiot_Core::event_post(const QVariant& value)
{
    QString event_id = "alarmLog";
    QString key = "EventList";
    return eventPost(event_id, key, value);
}
