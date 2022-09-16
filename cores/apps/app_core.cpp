/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_core.h"
#include "cfg_rwmain.h"

App_Core::App_Core(QObject *parent)
    : App_Start{parent}
{
    Shm::initShm();
    Cfg_ReadWrite::bulid();
    compileTime();
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
    QString str = cm::buildDateTime().toString("yyyy-MM-dd hh:mm:ss");
    qstrcpy(vers->coreCompileTime, str.toLatin1().data());
}
