/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "data_core.h"
#include "alarm_Updater.h"

Data_Core::Data_Core()
{
    QtConcurrent::run(this,&Data_Core::run);
}

Data_Core *Data_Core::bulid()
{
    static Data_Core* sington = nullptr;
    if(sington == nullptr) sington = new Data_Core();
    return sington;
}

void Data_Core::upAlarm()
{
    Alarm_Updater::bulid()->run();
}

void Data_Core::run()
{
    cm::mdelay(4444);
    while(isRun) {
        outletWork();
        cm::mdelay(400); tgWork();
        dualWork(); cm::mdelay(100);
        groupWork(); cm::mdelay(100);
        loopWork(); cm::mdelay(100);
        lineWork(); cm::mdelay(100);
        envWork(); cm::mdelay(100);
        upAlarm(); cm::mdelay(100);
    }
}
