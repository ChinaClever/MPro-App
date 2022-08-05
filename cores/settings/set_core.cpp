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

void Set_Core::writeAlarm()
{
    Cfg_ReadWrite::bulid()->writeAlarms();
}


QVariant Set_Core::getCfg(sCfgItem &it)
{
    QVariant res; switch (it.fc) {
    case SFnCode::Uuts: res = getUut(it.addr, it.type); break;
    case SFnCode::ESnmp: res = snmpCfg(it.type); break;
    case SFnCode::EPush: res = pushCfg(it.type); break;
    case SFnCode::EDevLogin: res = loginUsrPwd(it.type); break;
    case SFnCode::EGrouping: res = grouping(it.addr, it.type); break;
    case SFnCode::EGroupName: res = groupName(it.addr, it.type); break;
    case SFnCode::OutputName: res = outputName(it.addr, it.type); break;
    case SFnCode::ECfgNum: res = devCfgNum(it.addr, it.type); break;
    case SFnCode::EDevInfo: res = devInfoCfg(it.addr, it.type);  break;
    case SFnCode::EModbus: res = modbusCfg(it.type); break;
    case SFnCode::ERpc: res = rpcCfg(it.type); break;
    default: qDebug() << Q_FUNC_INFO << it.fc; break;
    }

    return res;
}

bool Set_Core::setParam(sCfgItem &it, const QVariant &v)
{
    bool ret = false; switch (it.fc) {
    case SFnCode::EGrouping: ret = groupingSet(it, v); break;
    case SFnCode::EGroupName: ret = groupNameSet(it, v); break;
    case SFnCode::OutputName: ret = outputNameSet(it, v); break;
    case SFnCode::Uuts: ret = setUut(it.type, v); break;
    case SFnCode::EPush: ret = pushSet(it.type, v); break;
    case SFnCode::ESnmp: ret = snmpSet(it.type, v); break;
    case SFnCode::ERpc: ret = rpcSet(it.type, v.toInt()); break;
    case SFnCode::EDevLogin: ret = loginSet(it.type, v); break;
    case SFnCode::EDevInfo: ret = setInfoCfg(it.addr, it.type, v.toInt()); break;
    case SFnCode::ECfgNum: ret = setCfgNum(it.addr, it.type, v.toInt()); break;
    case SFnCode::EModbus: ret = modbusSet(it.type, v.toInt()); break;
    default: qDebug() << Q_FUNC_INFO << it.fc; break;
    }

    return ret;
}


bool Set_Core::setCfg(sCfgItem &it, const QVariant &v)
{    
    bool ret = false;
    if(it.addr) {
        int num = cm::masterDev()->cfg.nums.slaveNum;
        if(num) ret = Cascade_Core::bulid()->masterSetCfg(it, v);
    } else {
        ret = setParam(it, v);
    }
    return ret;
}

bool Set_Core::setting(sDataItem &it)
{
    bool ret = true; if(it.rw) {
        if(it.addr) {
            int num = cm::masterDev()->cfg.nums.slaveNum;
            if(num) ret = Cascade_Core::bulid()->masterSeting(it);
        } else if(it.topic == DTopic::Relay) {
            ret = relaySet(it);
        } else {
            ret = setAlarm(it);
            if(ret) writeAlarm();
        }
    } else {
        ret = false;
        qDebug() << Q_FUNC_INFO;
    }

    return ret;
}


