#include "data_core.h"

Data_Core::Data_Core()
{

}

Data_Core *Data_Core::bulid()
{
    static Data_Core* sington = nullptr;
    if(sington == nullptr) sington = new Data_Core();
    return sington;
}

void Data_Core::run()
{
    while(isRun) {
        cm::mdelay(1100);
        loopWork(); cm::mdelay(100);
        lineWork(); cm::mdelay(100);
    }
}
