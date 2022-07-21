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

bool Set_Core::setString(sNumStrItem &it)
{
    bool ret = false; switch (it.fc) {
    case SFnCode::Grouping: ret = groupingSet(it); break;
    case SFnCode::GroupName: ret = groupNameSet(it); break;
    case SFnCode::OutputName: ret = outputNameSet(it); break;
    case SFnCode::Uuts: ret = setUut(it.id, it.str, it.txType); break;
    case SFnCode::EPush: ret = pushSet(it.id, it.str, it.txType); break;
    case SFnCode::ESnmp: ret = snmpSet(it.id, it.str, it.txType); break;
    case SFnCode::EDevLogin: ret = loginSet(it.id, it.str, it.txType); break;
    default: qDebug() << Q_FUNC_INFO << it.fc; break;
    }

    return ret;
}

QString Set_Core::getString(sNumStrItem &it)
{
    QString str; switch (it.fc) {
    case SFnCode::Uuts: str = getUut(it.addr, it.id); break;
    case SFnCode::ESnmp: str = snmpCfg(it.id); break;
    case SFnCode::EPush: str = pushCfg(it.id).toString(); break;
    case SFnCode::EDevLogin: str = loginUsrPwd(it.id); break;
    case SFnCode::Grouping: str = grouping(it.addr, it.id); break;
    case SFnCode::GroupName: str = groupName(it.addr, it.id); break;
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
    case SFnCode::EModbus: ret = modbusSet(it.id, it.value, it.txType); break;
    case SFnCode::EPush: ret = pushSet(it.id, it.value, it.txType); break;
    case SFnCode::ERpc: ret = rpcSet(it.id, it.value, it.txType); break;
    default: qDebug() << Q_FUNC_INFO << it.fc; break;
    } if(ret) writeAlarm();

    return ret;
}

int Set_Core::getNumber(sNumStrItem &it)
{
    int ret = 0; switch (it.fc) {
    case SFnCode::ECfgNum: ret = devCfgNum(it.addr, it.id); break;
    case SFnCode::EDevInfo: ret = devInfoCfg(it.addr, it.id);  break;
    case SFnCode::EPush: ret = pushCfg(it.id).toInt(); break;
    case SFnCode::EModbus: ret = modbusCfg(it.id); break;
    case SFnCode::ERpc: ret = rpcCfg(it.id); break;
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
            if(ret) writeAlarm();
        }
    } else {
        ret = false;
        qDebug() << Q_FUNC_INFO;
    }

    return ret;
}

