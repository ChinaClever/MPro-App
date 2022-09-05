/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_core.h"
#include "cascade_core.h"
#include "log_core.h"

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
    case SFnCode::ENtp: res = ntpCfg(it.fc); break;
    case SFnCode::EWeb: res = webCfg(it.fc); break;
    case SFnCode::ESmtp: res = smtpCfg(it.fc); break;
    case SFnCode::ESnmp: res = snmpCfg(it.fc); break;
    case SFnCode::EPush: res = pushCfg(it.fc); break;
    case SFnCode::EMqtt: res = mqttCfg(it.fc); break;
    case SFnCode::EDevLogin: res = loginUsrPwd(it.fc); break;

    case SFnCode::EOutput: case SFnCode::EGroup:
    case SFnCode::EDual: res = outputCfg(it); break;
    case SFnCode::EGrouping: res = grouping(it.addr, it.fc); break;
    case SFnCode::OutputName: res = outputName(it.addr, it.fc); break;
    case SFnCode::EVersion: res = softwareVersion(it.addr, it.fc); break;

    case SFnCode::ECfgNum: res = devCfgNum(it); break;
    case SFnCode::EDevInfo: res = devInfoCfg(it.addr, it.fc); break;
    case SFnCode::EINet: res = netAddrCfg(it.fc, it.sub); break;
    case SFnCode::ESercret: res = getSercret(it.fc); break;
    case SFnCode::ETlsCert: res = getTlsCert(it.fc); break;
    case SFnCode::EModbus: res = modbusCfg(it.fc); break;
    case SFnCode::Uuts: res = getUut(it.addr, it.fc); break;
    case SFnCode::ERpc: res = rpcCfg(it.fc); break;
    case SFnCode::ELog: res = Log_Core::bulid()->logFun(it); break;
    default: qDebug() << Q_FUNC_INFO << it.type; break;
    }

    return res;
}

bool Set_Core::setParam(sCfgItem &it, const QVariant &v)
{
    bool ret = false; switch (it.type) {
    case SFnCode::EOutput: case SFnCode::EGroup:
    case SFnCode::EDual: ret = outputSet(it, v); break;
    case SFnCode::EGrouping: ret = groupingSet(it, v); break;
    case SFnCode::OutputName: ret = outputNameSet(it, v); break;

    case SFnCode::ENtp: ret = ntpSet(it.fc, v); break;
    case SFnCode::EWeb: ret = webSet(it.fc, v); break;
    case SFnCode::Uuts: ret = setUut(it.fc, v); break;
    case SFnCode::ESmtp: ret = smtpSet(it.fc, v); break;
    case SFnCode::EINet: ret = netAddrSet(it, v); break;
    case SFnCode::EPush: ret = pushSet(it.fc, v); break;
    case SFnCode::ESnmp: ret = snmpSet(it.fc, v); break;
    case SFnCode::EMqtt: ret = mqttSet(it.fc, v); break;
    case SFnCode::ERpc: ret = rpcSet(it.fc, v.toInt()); break;
    case SFnCode::EDevLogin: ret = loginSet(it.fc, v); break;
    case SFnCode::ESercret: ret = setSercret(it.fc, v); break;
    case SFnCode::EDevInfo: ret = setInfoCfg(it.addr, it.fc, v.toInt()); break;
    case SFnCode::ECfgNum: ret = setCfgNum(it, v.toInt()); break;
    case SFnCode::EModbus: ret = modbusSet(it.fc, v.toInt()); break;
    case SFnCode::ECmd: ret = system(v.toByteArray().data()); break;
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


