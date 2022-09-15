/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "data_core.h"
#include "alarm_Updater.h"

Data_Core::Data_Core()
{

}

Data_Core *Data_Core::bulid()
{
    static Data_Core* sington = nullptr;
    if(sington == nullptr) {
        sington = new Data_Core();
        QtConcurrent::run(sington,&Data_Core::run);
    }
    return sington;
}

void Data_Core::upAlarm()
{
    static uint cnt = 1;
    if(!(++cnt % 5)) {
        Alarm_Updater::bulid()->run();
    }


    /////////============
    for(int i=0; i<24; ++i) {
        char *ptr = mDev->output.relay.timingOn[i];
        qstrcpy(ptr, "2020-10-01 11:10:00");
        ptr = mDev->output.relay.timingOff[i];
        qstrcpy(ptr, "2020-10-01 18:10:00");

        ptr = mDev->group.relay.timingOn[i];
        qstrcpy(ptr, "2021-10-01 11:10:00");
        ptr = mDev->group.relay.timingOff[i];
        qstrcpy(ptr, "2021-10-01 18:10:00");

        ptr = mDev->dual.relay.timingOn[i];
        qstrcpy(ptr, "2022-10-01 11:10:00");
        ptr = mDev->dual.relay.timingOff[i];
        qstrcpy(ptr, "2022-10-01 18:10:00");
    }

}

void Data_Core::run()
{
    while(isRun) {
        outletWork();
        cm::mdelay(400);tgWork();
        dualWork(); cm::mdelay(100);
        groupWork(); cm::mdelay(100);
        loopWork(); cm::mdelay(100);
        lineWork(); cm::mdelay(100);
        envWork(); cm::mdelay(100);
        upAlarm(); cm::mdelay(100);
    }
}
