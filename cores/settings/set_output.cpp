/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_output.h"
#include "op_core.h"
#include "cascade_core.h"
#include "data_core.h"

Set_Output::Set_Output()
{

}

void Set_Output::relayOpLog(const sDataItem &it)
{
    QString str = QStringLiteral("全部"); if(cm::cn()){
        if(it.id) str = QStringLiteral("第%1").arg(it.id);
    } else {
        str = "all ";  if(it.id) str = QString::number(it.id) =" ";
    }

    switch (it.subtopic) {
    case DSub::Value:
        if(cm::cn()) {
            if(it.type == DType::Group) str += QStringLiteral("组开关 ");
            else if(it.type == DType::Dual) str += QStringLiteral("双电源开关 ");
            else if(it.type == DType::Dual) str += QStringLiteral("双电源开关 ");
            else str += QStringLiteral(" 输出位继电器 ");
            if(it.value) str += QStringLiteral("闭合"); else str += QStringLiteral("断开");
        } else {
            if(it.type == DType::Group) str += "group switch ";
            else if(it.type == DType::Dual) str += "group switch ";
            else if(it.type == DType::Dual) str += "Dual power switch ";
            else str += "Output position relay ";
            if(it.value) str += "close"; else str += "break";
        } break;
    case DSub::Rated:
        if(cm::cn()) {
            str += QStringLiteral("输出位继电器模式切换 ");
            if(sRelay::OffALarm == it.value) str += QStringLiteral("断开报警模式");
            else str += QStringLiteral("默认");
        } else {
            str += "Output bit relay mode switching ";
            if(sRelay::OffALarm == it.value) str += "Disconnect alarm mode";
            else str += "default ";
        } break;
    case DSub::UpDelay:
        if(cm::cn()) str += QStringLiteral("输出位继电器上电延时，修改为: ");
        else str += "Output position relay power-on delay, modified to ";
        str += QString::number(it.value) +"s"; break;
    case DSub::ResetDelay:
        if(cm::cn()) str += QStringLiteral("输出位继电器复位延时，修改为 ");
        else str += "Output bit relay reset delay, modified to ";
        str += QString::number(it.value) +"s"; break;
    case DSub::OverrunOff:
        if(cm::cn()) {
            str += QStringLiteral("输出位超限断电，修改为 ");
            if(it.value) str += QStringLiteral("启用"); else str += QStringLiteral("禁用");
        } else {
            str += "Output bit out of limit power outage, modified to ";
            if(it.value) str += "Enable"; else str += "Disable";
        } break;

    case DSub::TimingEn:
        if(cm::cn()) {
            str += QStringLiteral("输出位定时功能，修改为");
            if(it.value) str += QStringLiteral("启用"); else str += QStringLiteral("禁用");
        } else{
            str += "Output bit timing function, modified to ";
            if(it.value) str += "Enable"; else str += "Disable";
        } break;
    case DSub::RelayEn:
        if(cm::cn()){
            str += QStringLiteral("关键设备保护，修改为 ");
            if(it.value) str += QStringLiteral("启用"); else str += QStringLiteral("禁用");
        } else {
            str += "Protection of critical equipment, modified to ";
            if(it.value) str += "Enable"; else str += "Disable";
        } break;
    case DSub::Relays: { int start = it.type; int end = start + it.id;
        if(cm::cn()) { str = QStringLiteral("第%１至%2 ").arg(start).arg(end-1);
            if(it.value) str += QStringLiteral("闭合"); else str += QStringLiteral("断开");
        } else {
            str = QStringLiteral(" %１ to %2 ").arg(start).arg(end-1);
            if(it.value) str += "close"; else str += "break";
        } break;}
    default: cout << it.subtopic; break;
    }

    sEventItem db;
    db.event_content = str;
    db.event_type = opSrc(it.addr, it.txType);
    if(cm::cn()) db.event_type += QStringLiteral("继电器 控制");
    else db.event_type += QStringLiteral("relay control");
    Log_Core::bulid()->append(db);
}

bool Set_Output::outputCtrl(const sDataItem &unit)
{
    if(unit.value > 2) return false;
    bool ret = true; int id = unit.id; if(id) id--;
    sRelayUnit *it = &(cm::masterDev()->output.relay);
    if(unit.type == DType::Dual) it = &(cm::masterDev()->dual.relay);
    if((0==it->disabled[id]) && (unit.value < 3)) { /* || (unit.txType == DTxType::TxWeb)*/
        if(id < it->size) {OP_Core::bulid()->relayCtrl(unit.id, unit.value);
            if(unit.value) it->cnt[id] += 1;
            it->sw[id] = unit.value;
        } else ret = false;
    } else ret = false;

    return ret;
}

bool Set_Output::outputsCtrl(const sDataItem &unit)
{
    sRelayUnit *it = &(cm::masterDev()->output.relay);
    bool ret = false; int start = unit.type-1; int end = start + unit.id;
    if(unit.type == DType::Dual) it = &(cm::masterDev()->dual.relay);    
    for(int i=start; i<end; ++i) {
        if((0==it->disabled[i]) && (unit.value < 2) && (i < it->size)){ /* || (unit.txType == DTxType::TxWeb)*/
            ret = true; if(unit.value) it->cnt[i] += 1;
        } else {ret = false; break;}
    }

    if(ret && unit.id) OP_Core::bulid()->relaysCtrl(start, end, unit.value);
    return ret;
}

bool Set_Output::groupCtrl(const sDataItem &unit)
{
    QList<int> ids;
    bool ret = true; int id = unit.id; if(id) {
        ids = Data_Core::bulid()->outletByGroup(id-1);
    } else {
        for(int i=0; i<GROUP_NUM; ++i)
            ids << Data_Core::bulid()->outletByGroup(i);
    }

    sRelayUnit *relay = &(cm::masterDev()->output.relay);
    foreach (const auto &i, ids){
        if(relay->disabled[i]) ids.removeOne(i);
        else if(unit.value) relay->cnt[i] += 1;
    }

    sRelayUnit *it = &(cm::masterDev()->group.relay);
    if(0==it->disabled[id] && (unit.value < 2)) { /* || unit.txType == DTxType::TxWeb*/        
        OP_Core::bulid()->relaysCtrl(ids, unit.value);
    } else ret = false;

    return ret;
}

bool Set_Output::relaySet(sDataItem &unit)
{
    bool ret = true;
    switch (unit.subtopic) {
    case DSub::Value: if(unit.type == DType::Group) ret = groupCtrl(unit); else ret = outputCtrl(unit); break;
    case DSub::Relays: ret = outputsCtrl(unit); break;
    case DSub::UpDelay: OP_Core::bulid()->setDelay(unit.id, unit.value); //break;
    default: ret = upMetaData(unit); Cfg_Core::bulid()->writeAlarms(); break;
    } if(ret) relayOpLog(unit);

    //if(unit.type == DType::Dual) {
    //    sDataItem it = unit; it.addr += 1;
    //    ret = Cascade_Core::bulid()->masterSeting(it);
    //}

    return ret;
}

QString Set_Output::outputCfg(const sCfgItem &it)
{
    uint addr = it.addr;
    if(addr==0xff) addr = 0;
    sDevData *dev = cm::devData(addr);
    QString res; int id = it.id; if(id) id--;
    sObjData *obj = nullptr;

    switch (it.type) {
    case SFnCode::EOutput: obj = &(dev->output); break;
    case SFnCode::EGroup: obj = &(dev->group); break;
    case SFnCode::EDual: obj = &(dev->dual); break;
    default: cout << it.type; return res;
    }

    switch (it.fc) {
    case 1: res = obj->name[id]; break;
    case 2: res = obj->relay.timingOn[id]; break;
    case 3: res = obj->relay.timingOff[id]; break;
    default: cout << it.fc; break;
    }

    return res;
}


QString Set_Output::grouping(int addr, int id)
{
    QString res; if(addr == 0xff) addr = 0;
    QList<int> ids = Data_Core::bulid()->outletByGroup(id-1, addr);
    foreach(auto &i, ids) res += QString::number(i+1) +";";
    return res;
}


QString Set_Output::outputName(int addr, int id)
{    
    sCfgItem it; it.fc = 1; it.addr = addr;
    it.type = SFnCode::EOutput; it.id = id;
    return outputCfg(it);
}

void Set_Output::opNameLog(const sCfgItem &it, const QVariant &v)
{    
    QString str = QStringLiteral("全部");
    if(cm::cn()){
        if(it.fc) str = QStringLiteral("第%1").arg(it.fc);
        str += QStringLiteral("名称修改为: %1").arg(v.toString());
    } else {
        str = "all ";
        if(it.fc) str = QString::number(it.fc);
        str += " change the name to " + v.toString();
    }

    QString op; switch (it.type) {
    case SFnCode::EOutput: if(cm::cn()) op += QStringLiteral("输出位名称"); else op += "Output bit name"; break;
    case SFnCode::EGroup: if(cm::cn()) op += QStringLiteral("组名称"); else op += "Group name"; break;
    case SFnCode::EDual: if(cm::cn()) op += QStringLiteral("机架名称"); else op += "Rack name"; break;
    default: cout << it.type; break;
    }

    sEventItem db;
    db.event_content = str;
    db.event_type = opSrc(it.addr, it.txType);
    db.event_type += op;
    Log_Core::bulid()->append(db);
}

bool Set_Output::outputNameSet(sCfgItem &it, const QVariant &v)
{
    it.type = SFnCode::EOutput;
    it.id = it.fc; it.fc = 1;
    return outputSet(it, v);
}

bool Set_Output::groupSet(sCfgItem &it, const QVariant &v)
{
    uint addr = it.addr; if(addr==0xff) addr = 0;
    sDevData *dev = cm::devData(addr); if(it.fc) it.fc -= 1;
    uchar *ptr = dev->cfg.nums.group[it.fc]; bool ret = true;
    if(it.id < OUTPUT_NUM) ptr[it.id-1] = v.toInt();
    else {cout << it.id; ret = false;}
    if(ret) Cfg_Core::bulid()->groupWrite();
    return ret;
}

bool Set_Output::groupingSet(sCfgItem &it, const QVariant &v)
{
    uint addr = it.addr; if(addr==0xff) addr = 0;
    QStringList strs = v.toString().simplified().split(";");
    sDevData *dev = cm::devData(addr); if(it.fc) it.fc -= 1;
    uchar *ptr = dev->cfg.nums.group[it.fc]; bool ret = true;
    memset(ptr, 0, OUTPUT_NUM);
    foreach(auto &str, strs) {
        int id = str.toInt(); if(id) id -= 1; ptr[id] = 1;
    } if(strs.size()) Cfg_Core::bulid()->groupWrite(); else ret = false;

    return ret;
}

bool Set_Output::outputSetById(sCfgItem &it, const QVariant &v)
{
    uint addr = it.addr; if(addr==0xff) addr = 0;
    QString prefix; bool res=true; int id = it.id;
    sObjData *obj = nullptr; char *ptr = nullptr;
    sDevData *dev = cm::devData(addr);

    switch (it.type) {
    case SFnCode::EOutput: obj = &(dev->output); prefix = "OutputName"; break;
    case SFnCode::EGroup: obj = &(dev->group); prefix = "GroupName"; break;
    case SFnCode::EDual: obj = &(dev->dual); prefix = "DualName"; break;
    default: res = false; cout << it.type; return res;
    }

    switch (it.fc) {
    case 1: ptr = obj->name[id]; break;
    case 2: ptr = obj->relay.timingOn[id]; break;
    case 3: ptr = obj->relay.timingOff[id]; break;
    default: res = false; cout << it.fc; break;
    } //cout << it.type << it.fc << v;

    if(ptr){
        QByteArray array = v.toByteArray();
        qstrncpy(ptr, array.data(), NAME_SIZE);
        if(it.fc == 1) {
            QString key = QString::number(id+1);
            Cfg_Com *cfg = Cfg_Com::bulid();
            cfg->writeCfg(key, v, prefix);
        }
    }
    return res;
}

bool Set_Output::outputSet(sCfgItem &it, const QVariant &v)
{
    int id = it.id; bool ret = true; if(it.id){
        it.id--; ret = outputSetById(it, v);
    } else {
        sObjData *obj = nullptr;
        uint addr = it.addr; if(addr==0xff) addr = 0;
        sDevData *dev = cm::devData(addr);
        switch (it.type) {
        case SFnCode::EOutput: obj = &(dev->output); break;
        case SFnCode::EGroup: obj = &(dev->group); break;
        case SFnCode::EDual: obj = &(dev->dual); break;
        default: cout << it.type; return ret;
        }
        for(int i=0; i<obj->size; ++i) {
            it.id = i; ret = outputSetById(it, v);
        }
    }

    if(it.fc == 1){it.id = id; opNameLog(it, v); }
    else Cfg_Core::bulid()->writeAlarms();
    if(it.type == DType::Dual) {
        sCfgItem unit = it; unit.addr += 1;
        ret = Cascade_Core::bulid()->masterSetCfg(unit, v);
    }

    return ret;
}

