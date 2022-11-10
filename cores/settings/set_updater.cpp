/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_updater.h"
#include "cascade_core.h"
#include "log_core.h"
#include "cfg_app.h"
#include "op_core.h"

Set_Updater::Set_Updater()
{

}

QVariant Set_Updater::otaStatus(sCfgItem &cfg)
{
    sOtaUpIt *it = nullptr;
    sOtaUpdater *ota = &(cm::dataPacket()->ota);
    QVariant res; switch (cfg.fc) {
    case 1: res = ota->work;  break;
    case 2: it = &ota->usb;  break;
    case 3: it = &ota->net;  break;
    case 4: it = &ota->web;  break;
    case 6: it = &ota->slave;  break;
    case 7: it = &ota->outlet; break;
    case 8: res = ota->host;  break;
    default: cout << it; break;
    }

    if(it) {
        switch (cfg.id) {
        case 1: res = it->isRun; break;
        case 2: res = it->subId; break;
        case 3: res = it->progress; break;
        case 4: res = it->results[cfg.addr]; break;
        default: cout << cfg.id; break;
        }
    }

    return res;
}

void Set_Updater::ota_log(int fc)
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

bool Set_Updater::ota_cascade(const QString &fn)
{
    bool ret = false;
    int num = cm::masterDev()->cfg.nums.slaveNum;
    sParameter *ptr = &(cm::masterDev()->cfg.param);
    uchar dm = ptr->devMode; if((dm>0) && (dm<3)) {
        if(ptr->cascadeAddr==0) {
            for(int i=1; i<num; ++i) {
                if(cm::devData(i)->offLine) ret = true;
            } if(ret) {
                sOtaFile it; it.fc = 1; QString path = fn;
                it.file = fn.split("/").last();
                it.path = path.remove(it.file);
                it.md5 = fn.split(".").last();
                it.size = File::size(fn);
                if(it.md5 != 32) it.md5 = File::md5(fn);
                Cascade_Core::bulid()->ota_start(it);
                setbit(cm::dataPacket()->ota.work, DOta_Slave);
            }
        }
    }
    return ret;
}

bool Set_Updater::ota_outlet(int fc)
{    
    QString dir = "/usr/data/updater/clever/outlet/";
    if(fc == DOta_Usb) dir = "";//////////////=============="
    QStringList fns = File::entryList(dir); bool ret = false;
    foreach (const auto &fn, fns) {
        if((fn == ".") || (fn == "..")) continue;
        setbit(cm::dataPacket()->ota.work, DOta_Outlet);
        OP_Core::bulid()->ota_start(dir+fn);
        ret = true; break;
    }

    return ret;
}

int Set_Updater::ota_updater(int fc, const QVariant &v)
{
    bool ret = false;
    fc -= 10; switch (fc) {
    case DOtaCode::DOta_Usb:  break;
    case DOtaCode::DOta_Net:  break;
    case DOtaCode::DOta_Web:  break;
    default: break;
    }

    QString fn = v.toString();
    if(fn.size()) {
        ota_log(fc); ret |= ota_outlet(fc);
        if(fc != DOta_Usb) ret |= ota_cascade(fn);
    }
    cout << fc << v << (ret?1:0);
    return ret?1:0;
}
