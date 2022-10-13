/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_updater.h"
#include "cascade_core.h"
#include "log_core.h"
#include "cfg_app.h"

Set_Updater::Set_Updater()
{

}

QVariant Set_Updater::otaStatus(int fc)
{
    sOtaUpdater *ota = &(cm::dataPacket()->ota);
    QVariant res; switch (fc) {
    case 1: res = ota->isRun;  break;
    case 2: res = ota->subId;  break;
    case 3: res = ota->progress;  break;
    case 4: res = ota->isOk;  break;
    case 5: res = ota->host;  break;
    default: cout << fc; break;
    }
    return res;
}

void Set_Updater::ota_log()
{
    sOtaItem it; sAppVerIt ver;
    Cfg_App cfg("/usr/data/updater/clever/");
    bool ret = cfg.app_unpack(ver);
    if(ret) {
        it.ver = ver.ver;
        it.md5 = ver.md5;
        it.remark = ver.remark;
        it.oldVersion = ver.oldVersion;
        it.releaseDate = ver.releaseDate;
        Log_Core::bulid()->append(it);
    }
}

void Set_Updater::ota_cascade(const QString &fn)
{
    sParameter *ptr = &(cm::masterDev()->cfg.param);
    uchar dm = ptr->devMode; if((dm>0) && (dm<3)) {
        if(ptr->cascadeAddr==0) {
            sOtaFile it; it.fc = 1; QString path = fn;
            it.file = fn.split("/").last();
            it.path = path.remove(it.file);
            it.md5 = fn.split(".").last();
            it.size = File::size(fn);
            if(it.md5 != 32) it.md5 = File::md5(fn);
            Cascade_Core::bulid()->ota_start(it);
        }
    }
}

void Set_Updater::ota_outlet()
{

}

bool Set_Updater::ota_updater(int fc, const QVariant &v)
{
    cout << fc << v;
    QString fn = v.toString();
    if(fn.size()) {
        ota_cascade(fn);
        ota_outlet();
        ota_log();
    }

    return true;
}
