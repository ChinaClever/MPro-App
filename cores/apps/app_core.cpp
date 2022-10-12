/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_core.h"
#include "cfg_core.h"

App_Core::App_Core(QObject *parent)
    : App_Start{parent}
{
#if (QT_VERSION > QT_VERSION_CHECK(5,15,0))
    Shm::initShm();
#endif
    compileTime();
    Cfg_Core::bulid();
}

App_Core *App_Core::bulid(QObject *parent)
{
    static App_Core* sington = nullptr;
    if(sington == nullptr) {
        sington = new App_Core(parent);
    }
    return sington;
}

void App_Core::compileTime()
{
    sVersions *vers = &(cm::masterDev()->cfg.vers);
    cm::buildDateTime(vers->compileDate);
}
