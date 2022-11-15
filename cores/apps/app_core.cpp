/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_core.h"
#include "cfg_core.h"
#include "cfg_app.h"
#include "log_core.h"

App_Core::App_Core(QObject *parent)
    : App_Start{parent}
{
#if (QT_VERSION > QT_VERSION_CHECK(5,15,0))
    Shm::initShm();
#else
    initVer();
#endif
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
    cm::dataPacket()->mem_size = sizeof(sDataPacket);
    sVersions *vers = &(cm::masterDev()->cfg.vers);
    cm::buildDateTime(vers->compileDate);
}

void App_Core::initVer()
{
    sVersions *ver = &(cm::masterDev()->cfg.vers);
    sAppVerIt it; Cfg_App cfg("/usr/data/clever/");
    bool ret = cfg.app_unpack(it); if(ret) {
        cm::buildDateTime(ver->compileDate);
        qstrcpy(ver->ver, it.ver.toUtf8().data());
        qstrcpy(ver->md5, it.md5.toUtf8().data());
        qstrcpy(ver->usr, it.usr.toUtf8().data());
        qstrcpy(ver->dev, it.dev.toUtf8().data());
        qstrcpy(ver->remark, it.remark.toUtf8().data());
        qstrcpy(ver->oldVersion, it.oldVersion.toUtf8().data());
        qstrcpy(ver->releaseDate, it.releaseDate.toUtf8().data());
        qstrcpy(ver->upgradeDate, it.upgradeDate.toUtf8().data());
    } else cout << CFG_APP << "error";

    sOtaItem ota;
    ota.md5 = "dm5";
    ota.ver = "ver";
    ota.oldVersion = "old";
    ota.releaseDate = "releaseDate";
    ota.remark = "remark";
    Log_Core::bulid()->append(ota);

}

