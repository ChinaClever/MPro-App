#include "data_core.h"
#include "alarm_Updater.h"

Data_Core::Data_Core()
{

}

Data_Core *Data_Core::bulid()
{
    static Data_Core* sington = nullptr;
    if(sington == nullptr) sington = new Data_Core();
    return sington;
}

void Data_Core::upAlarm()
{
    static uint cnt = 1;
    if(!(++cnt % 5)) {
        Alarm_Updater::bulid()->run();
    }
}

void Data_Core::run()
{
    while(isRun) {
        cm::mdelay(1100);
        loopWork(); cm::mdelay(100);
        lineWork(); cm::mdelay(100);
        upAlarm(); cm::mdelay(100);
    }
}
