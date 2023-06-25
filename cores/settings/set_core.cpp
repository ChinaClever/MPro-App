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
    Cfg_Core::bulid();
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
    Cfg_Core::bulid()->writeAlarms();
}


QVariant Set_Core::getCfg(sCfgItem &it)
{
    QVariant res; switch (it.type) {
    case SFnCode::EBR: res = backups(it.fc); break;
    case SFnCode::EOta: res = otaStatus(it); break;
    case SFnCode::ESsh: res = sshCfg(it.fc); break;
    case SFnCode::ENtp: res = ntpCfg(it.fc); break;
    case SFnCode::EWeb: res = webCfg(it.fc); break;
    case SFnCode::EAIOT: res = aiotCfg(it.fc); break;
    case SFnCode::EMqtt: res = mqttCfg(it.fc); break;
    case SFnCode::EAmqp: res = amqpCfg(it.fc); break;
    case SFnCode::EPush: res = pushCfg(it.fc, it.id); break;
    case SFnCode::ESnmp: res = snmpCfg(it.fc, it.id); break;
    case SFnCode::ESmtp: res = smtpCfg(it.fc, it.id); break;
    case SFnCode::EScript: res = scriptCfg(it.fc, it.id); break;
    case SFnCode::EDgsNet: res = net_diagnoseCfg(it.fc); break;

    case SFnCode::EOutput: case SFnCode::EGroup:
    case SFnCode::EDual: res = outputCfg(it); break;
    case SFnCode::EGrouping: res = grouping(it.addr, it.fc); break;
    case SFnCode::EDevLogin: res = loginUsrPwd(it.fc, it.id); break;
    case SFnCode::OutputName: res = outputName(it.addr, it.fc); break;
    case SFnCode::EVersion: res = softwareVersion(it.addr, it.fc); break;

    case SFnCode::ELdap: res = ldapCfg(it.fc); break;
    case SFnCode::EODBC: res = odbcCfg(it.fc); break;
    case SFnCode::ELogCfg: res = logCfg(it.fc); break;
    case SFnCode::ECfgNum: res = devCfgNum(it); break;
    case SFnCode::ERedis: res = redisCfg(it.fc); break;
    case SFnCode::ERadius: res = raduisCfg(it.fc); break;
    case SFnCode::ESysLog: res = syslogCfg(it.fc); break;
    case SFnCode::EModbus: res = modbusCfg(it.fc); break;
    case SFnCode::EDgsDev: res = downDiagnostics(); break;
    case SFnCode::ESercret: res = getSercret(it.fc); break;
    case SFnCode::ETlsCert: res = getTlsCert(it.fc); break;
    case SFnCode::EINet: res = netAddrCfg(it.fc, it.id); break;
    case SFnCode::EWhiteList: res = whiteListCfg(it.fc); break;
    case SFnCode::EDevInfo: res = devInfoCfg(it.addr, it.fc); break;
    case SFnCode::Uuts: res = getUut(it.addr, it.fc); break;
    case SFnCode::EPro: res = proStartupLog(it); break;
    case SFnCode::ERpc: res = rpcCfg(it.fc); break;
    case SFnCode::ELog: res = Log_Core::bulid()->logFun(it); break;
    case SFnCode::EAlarm: res = Alarm_Log::bulid()->getCurrentAlarm(it.fc); break;
    default: cout << it.type; break;
    }

    return res;
}

int Set_Core::setParam(sCfgItem &it, const QVariant &v)
{
    int ret = false; switch (it.type) {
    case SFnCode::EOutput: case SFnCode::EGroup:
    case SFnCode::EDual: ret = outputSet(it, v); break;
    case SFnCode::EGroupSet: ret = groupSet(it, v); break;
    case SFnCode::EGrouping: ret = groupingSet(it, v); break;
    case SFnCode::OutputName: ret = outputNameSet(it, v); break;
    case SFnCode::EWhiteList: ret = setWhiteList(it.fc, v); break;
    case SFnCode::EDgsNet: ret = net_diagnoseSet(it.fc, v); break;
    case SFnCode::EThreshold: ret = thresholdSlave(it.fc); break;

    case SFnCode::ESys: ret = syscmd(it.fc); break;
    case SFnCode::EBR: ret = restores(it.fc, v); break;
    case SFnCode::ESsh: ret = sshSet(it.fc, v); break;
    case SFnCode::ENtp: ret = ntpSet(it.fc, v); break;
    case SFnCode::EWeb: ret = webSet(it.fc, v); break;
    case SFnCode::Uuts: ret = setUut(it.fc, v); break;
    case SFnCode::ELdap: ret = ldapSet(it.fc, v); break;
    case SFnCode::EODBC: ret = odbcSet(it.fc, v); break;
    case SFnCode::EINet: ret = netAddrSet(it, v); break;
    case SFnCode::EMqtt: ret = mqttSet(it.fc, v); break;
    case SFnCode::EAmqp: ret = amqpSet(it.fc, v); break;        
    case SFnCode::EAIOT: ret = aiotSet(it.fc, v); break;
    case SFnCode::EOta: ret = ota_updater(it.fc, v); break;
    case SFnCode::ERedis: ret = redisSet(it.fc, v); break;
    case SFnCode::ELogCfg: ret = logSet(it.fc, v); break;
    case SFnCode::ERadius: ret = raduisSet(it.fc, v); break;
    case SFnCode::ESysLog: ret = syslogSet(it.fc, v); break;
    case SFnCode::ERpc: ret = rpcSet(it.fc, v.toInt()); break;
    case SFnCode::ETlsCert: ret = setTlsCert(it.fc, v); break;
    case SFnCode::EScript: ret = scriptSet(it.fc, it.id, v); break;
    case SFnCode::EDevLogin: ret = loginSet(it.fc, v, it.id); break;
    case SFnCode::ESercret: ret = setSercret(it.fc, v); break;
    case SFnCode::ESnmp: ret = snmpSet(it.fc, it.id, v); break;
    case SFnCode::ESmtp: ret = smtpSet(it.fc, it.id, v); break;
    case SFnCode::EPush: ret = pushSet(it.fc, it.id, v); break;
    case SFnCode::ECfgNum: ret = setCfgNum(it, v.toInt()); break;
    case SFnCode::EDevInfo: ret = setInfoCfg(it.fc, v.toInt()); break;
    case SFnCode::EModbus: ret = modbusSet(it.fc, v.toInt()); break;
    case SFnCode::ECmd: ret = system(v.toByteArray().data()); break;
    default: cout << it.type; break;
    }

    return ret;
}


int Set_Core::setCfg(sCfgItem &it, const QVariant &v)
{
    int ret = false;
    if(it.addr==0 || it.addr==0xff) {
        ret = setParam(it, v);
    }

    if(it.addr) {
        int num = cm::masterDev()->cfg.nums.slaveNum;
        if(num) ret = Cascade_Core::bulid()->masterSetCfg(it, v);        
    } if(it.type == SFnCode::EDual) { it.addr += 1; Cascade_Core::bulid()->masterSetCfg(it, v);}

    return ret;
}

int Set_Core::setting(sDataItem &it)
{
    int ret = true;

    if(it.rw) {
        if(it.addr==0 || it.addr==0xff) {
            if(it.topic == DTopic::Relay) {
                ret = relaySet(it);
            } else {
                ret = setAlarm(it);
            }
            if(ret) writeAlarm();
        }

        if(it.addr) {
            int num = cm::masterDev()->cfg.nums.slaveNum;
            if(num) ret = Cascade_Core::bulid()->masterSeting(it);
        } if((it.topic != DTopic::Relay) && ret) setAlarmLog(it);
        if(it.type == DType::Dual) { it.addr += 1; Cascade_Core::bulid()->masterSeting(it);}
    } else {
        ret = false;
        cout << it.rw;
    }

    return ret;
}


