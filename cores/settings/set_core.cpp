/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_core.h"
#include "cascade_core.h"
#include "set_ssdp.h"

Set_Core::Set_Core()
{
    Cfg_ReadWrite::bulid();
}

Set_Core *Set_Core::bulid()
{
    static Set_Core* sington = nullptr;
    if(sington == nullptr) {
        sington = new Set_Core();
    }
    return sington;
}

void Set_Core::writeSettings()
{
    Cfg_ReadWrite::bulid()->writeSettings();
}

bool Set_Core::setString(sNumStrItem &it)
{
    bool ret = false; switch (it.fc) {
    case SFnCode::OutputName: ret = outputNameSet(it); break;
    case SFnCode::Uuts: ret = setUut(it.id, it.str, it.txType); break;
    case SFnCode::EDevLogin: ret = loginSet(it.id, it.str, it.txType); break;
    default: qDebug() << Q_FUNC_INFO << it.fc; break;
    }

    return ret;
}

QString Set_Core::getString(sNumStrItem &it)
{
    QString str; switch (it.fc) {
    case SFnCode::Uuts: str = getUut(it.addr, it.id); break;
    case SFnCode::EDevLogin: str = loginUsrPwd(it.id); break;
    case SFnCode::OutputName: str = outputName(it.addr, it.id); break;
    default: qDebug() << Q_FUNC_INFO << it.fc; break;
    }

    return str;
}

bool Set_Core::setNumber(sNumStrItem &it)
{
    bool ret = false; switch (it.fc) {
    case SFnCode::ECfgNum: ret = setCfgNum(it.addr, it.id, it.value); break;
    case SFnCode::EDevInfo: ret = setInfoCfg(it.addr, it.id, it.value); break;
    default: qDebug() << Q_FUNC_INFO << it.fc; break;
    } if(ret) writeSettings();

    return ret;
}

int Set_Core::getNumber(sNumStrItem &it)
{
    int ret = 0; switch (it.fc) {
    case SFnCode::ECfgNum: ret = devCfgNum(it.addr, it.id); break;
    case SFnCode::EDevInfo: ret = devInfoCfg(it.addr, it.id);  break;
    default: qDebug() << Q_FUNC_INFO << it.fc; break;
    }

    return ret;
}

bool Set_Core::setNumStr(sNumStrItem &it)
{    
    bool ret = false; if(it.rw) {
        if(it.soi > 1) {
           ret = Set_Ssdp::bulid()->setNumStr(it);
        } else if(it.addr  || it.soi) {
            if(it.soi) it.addr = 0xFF;
            int num = cm::masterDev()->info.slaveNum;
            if(num) ret = Cascade_Core::bulid()->masterSetNumStr(it);
            if(it.soi) {it.addr = it.soi = 0; setNumStr(it);}
        } else {
            if(it.isDigit) ret = setNumber(it);
            else ret = setString(it);
        }
    }
    return ret;
}

QString Set_Core::getNumStr(sNumStrItem &it)
{
    QString str; if(!it.rw) {
        if(it.isDigit) str = QString::number(getNumber(it));
        else str = getString(it);
    } else qDebug() << Q_FUNC_INFO;
    return str;
}

bool Set_Core::setting(sDataItem &it)
{
    bool ret = true; if(it.rw) {
        if(it.soi > 1) {
           ret = Set_Ssdp::bulid()->setting(it);
        } else if(it.addr || it.soi) {
            if(it.soi) it.addr = 0xFF;
            int num = cm::masterDev()->info.slaveNum;
            if(num) ret = Cascade_Core::bulid()->masterSeting(it);
            if(it.soi) {it.addr = it.soi = 0; setting(it);}
        } else if(it.topic == DTopic::Relay) {
            ret = relaySet(it);
        } else {
            ret = setAlarm(it);
            if(ret) writeSettings();
        }
    } else {
        ret = false;
        qDebug() << Q_FUNC_INFO;
    }

    return ret;
}

