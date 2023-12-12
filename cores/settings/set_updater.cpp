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
        case 3: res = it->progress + it->reserve/10.0; break;
        case 4: res = it->progs[cfg.addr] + it->progs[DEV_NUM/2+cfg.addr]/10.0; break;
        case 5: res = it->results[cfg.addr]; break;
        default: cout << cfg.id; break;
        }
    }

    return res;
}

void Set_Updater::ota_log()
{
    QString dir = "/usr/data/clever/";
    sOtaItem it; sAppVerIt ver; Cfg_App cfg(dir);
    bool ret = cfg.app_unpack(ver);
    if(ret) {
        it.remark = "[OK] " + ver.remark;
        it.releaseDate = ver.releaseDate;
        it.ver = ver.ver; it.md5 = ver.md5;
        QString version = cm::masterDev()->cfg.vers.ver;
        if(version.isEmpty()) version = ver.oldVersion;
        it.oldVersion = version;
        Log_Core::bulid()->append(it);
        cfg.app_oldVersion(version);
    }
}

bool Set_Updater::ota_logErr(int fc, const QString &fn)
{
    bool ret = true; QString str;
    QString dir = "/tmp/updater/ota_apps/";
    sOtaItem it; if(QFile::exists(dir+"ver.ini")) {
        sAppVerIt ver; Cfg_App cfg(dir);
        cfg.app_unpack(ver); it.ver = ver.ver; it.md5 = ver.md5;
        QString version = cm::masterDev()->cfg.vers.ver;
        if(version.isEmpty()) version = ver.oldVersion;
        it.oldVersion = version;

        switch (fc) {
        case 401: if(cm::cn()) str = "未升级：目前软件版本高于升级升级";
            else str = "Not upgraded: The current software version is higher than the upgraded version";
            break;

        case 402: if(cm::cn()) str = "未升级：升级包中的目标设备型号不相符";
            else str = "Not upgraded: The target device model in the upgrade package does not match";
            break;

        case 403: if(cm::cn()) str = "未升级：升级包已损坏";
            else str = "Not upgraded: The upgrade package is corrupt";
            break;

        case 404: if(cm::cn()) str = "未升级：签名信息错误";
            else str = "Not upgraded: incorrect signature information";
            break;

        case 411: if(cm::cn()) str = "未升级：校验文件不存在";
            else str = "Not upgraded: The verification file does not exist";
            break;

        case 412: if(cm::cn()) str = "未升级：MD5校验码格式不对";
            else str = "Not upgraded: MD5 checksum format is incorrect";
            break;

        case 413: if(cm::cn()) str = "未升级：签名信息格式不对";
            else str = "Not upgraded: The signature information format is incorrect";
            break;

        default: if(cm::cn()) str = "未升级：升级包错误";
            else str = "Not upgraded: The upgrade package is corrupt";
            break;
        }
        it.remark = "[error] " + str;
        //it.oldVersion = ver.oldVersion;
        it.releaseDate = ver.releaseDate;
    } else {
        QString file = fn.split("/").last().remove(".zip");
        QStringList ls = file.split("_");
        if(ls.size() == 3) {
            it.ver = ls.at(1);
            it.md5 = "md5 error";
            it.remark = "updater error";
            it.oldVersion = "--- ---";
            it.releaseDate = ls.last();
        } ret = false;
    } Log_Core::bulid()->append(it);
    return ret;
}

bool Set_Updater::ota_cascade(const QString &fn)
{    
    int num = cm::masterDev()->cfg.nums.slaveNum;
    sParameter *ptr = &(cm::masterDev()->cfg.param);
    bool ret = false; if(ptr->devMode) {
        if(ptr->cascadeAddr==0) {
            for(int i=1; i<=num; ++i) {
                if(cm::devData(i)->offLine) ret = true;
            } if(ret) {
                sOtaFile it; it.fc = 1; QString path = fn;
                it.file = fn.split("/").last();
                it.path = path.remove(it.file);
                it.md5 = File::md5(fn);
                it.size = File::size(fn);
                Cascade_Core::bulid()->ota_start(it);
                setbit(cm::dataPacket()->ota.work, DOta_Slave);
            }
        }
    }
    return ret;
}

bool Set_Updater::ota_outlet()
{    
    QString dir = "/usr/data/clever/outlet/";
    QStringList fns = File::entryList(dir); bool ret = false;
    foreach (const auto &fn, fns) {
        if((fn == ".") || (fn == "..")) continue;
        setbit(cm::dataPacket()->ota.work, DOta_Outlet);
        OP_Core::bulid()->ota_start(dir+fn); cout << fns;
        ret = true; break;
    }

    return ret;
}

bool Set_Updater::ota_error(int fc, const QString &fn)
{
    bool ret = ota_logErr(fc, fn);
    if(ret) ret = ota_cascade(fn);
    return ret?1:0;
}

int Set_Updater::ota_updater(int fc, const QVariant &v)
{
    bool ret = false; switch (fc) {
    case DOtaCode::DOta_Usb: break;
    case DOtaCode::DOta_Net: break;
    case DOtaCode::DOta_Web: break;
    case DOtaCode::DOta_Rootfs: break;
    default: return ota_error(fc, v.toString());
    }

    QString fn = v.toString();
    if(fn.size()) {
        ota_log(); ret |= ota_outlet();
        if(fc != DOta_Usb) ret |= ota_cascade(fn);
    }
    cout << fc << v << (ret?1:0);
    return ret?1:0;
}
