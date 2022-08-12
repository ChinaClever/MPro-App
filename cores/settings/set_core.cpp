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
    QVariant res; switch (it.type) {
    case SFnCode::ESnmp: res = snmpCfg(it.fc); break;
    case SFnCode::EPush: res = pushCfg(it.fc); break;
    case SFnCode::EDevLogin: res = loginUsrPwd(it.fc); break;
    case SFnCode::EGrouping: res = grouping(it.addr, it.fc); break;
    case SFnCode::EGroupName: res = groupName(it.addr, it.fc); break;
    case SFnCode::OutputName: res = outputName(it.addr, it.fc); break;
    case SFnCode::EVersion: res = softwareVersion(it.addr, it.fc); break;
    case SFnCode::ETimingOn: res = outputTiming(it.addr, it.fc, 0); break;
    case SFnCode::ETimingOff: res = outputTiming(it.addr, it.fc, 1); break;
    case SFnCode::EGroupTimingOn: res = groupTiming(it.addr, it.fc, 0); break;
    case SFnCode::EGroupTimingOff: res = groupTiming(it.addr, it.fc, 1); break;

    case SFnCode::EDevInfo: res = devInfoCfg(it.addr, it.fc); break;
    case SFnCode::ECfgNum: res = devCfgNum(it.addr, it.fc); break;
    case SFnCode::ESercret: res = getSercret(it.fc); break;
    case SFnCode::EModbus: res = modbusCfg(it.fc); break;
    case SFnCode::Uuts: res = getUut(it.addr, it.fc); break;
    case SFnCode::ERpc: res = rpcCfg(it.fc); break;
    default: qDebug() << Q_FUNC_INFO << it.type; break;
    }

    return res;
}

bool Set_Core::setParam(sCfgItem &it, const QVariant &v)
{
    bool ret = false; switch (it.type) {
    case SFnCode::EGrouping: ret = groupingSet(it, v); break;
    case SFnCode::EGroupName: ret = groupNameSet(it, v); break;
    case SFnCode::OutputName: ret = outputNameSet(it, v); break;
    case SFnCode::ETimingOn: ret = setOutputTiming(it.addr, it.fc, 0, v); break;
    case SFnCode::ETimingOff: ret = setOutputTiming(it.addr, it.fc, 1, v); break;
    case SFnCode::EGroupTimingOn: ret = setGroupTiming(it.addr, it.fc, 0, v); break;
    case SFnCode::EGroupTimingOff: ret = setGroupTiming(it.addr, it.fc, 1, v); break;

    case SFnCode::Uuts: ret = setUut(it.fc, v); break;
    case SFnCode::EPush: ret = pushSet(it.fc, v); break;
    case SFnCode::ESnmp: ret = snmpSet(it.fc, v); break;
    case SFnCode::ERpc: ret = rpcSet(it.fc, v.toInt()); break;
    case SFnCode::EDevLogin: ret = loginSet(it.fc, v); break;
    case SFnCode::ESercret: ret = setSercret(it.fc, v); break;
    case SFnCode::EDevInfo: ret = setInfoCfg(it.addr, it.fc, v.toInt()); break;
    case SFnCode::ECfgNum: ret = setCfgNum(it.addr, it.fc, v.toInt()); break;
    case SFnCode::EModbus: ret = modbusSet(it.fc, v.toInt()); break;

    default: qDebug() << Q_FUNC_INFO << it.type; break;
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


