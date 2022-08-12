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
    QString str = QObject::tr("全部");
    if(it.id) str = QObject::tr("第%１ ").arg(it.id);
    switch (it.subtopic) {
    case DSub::Value:
        if(it.type == DType::Group) str += QObject::tr("组开关 ");
        else str += QObject::tr("输出位继电器 ");
        if(it.value) str += QObject::tr("闭合"); else str += QObject::tr("断开");
        break;
    case DSub::Rated:
        str += QObject::tr("输出位继电器模式切换 ");
        if(sRelay::OffALarm == it.value) str += QObject::tr("断开报警模式");
        else {str += QObject::tr("默认");} break;
    case DSub::UpDelay: str += QObject::tr("输出位继电器上电延时，修改为 %1s").arg(it.value); break;
    case DSub::ResetDelay: str += QObject::tr("输出位继电器复位延时，修改为 %1s").arg(it.value); break;
    case DSub::OverrunOff: str += QObject::tr("输出位超限断电，修改为 %1s").arg(it.value); break;
    case DSub::TimingEn: str += QObject::tr("输出位定时功能，修改为 %1s").arg(it.value); break;
    case DSub::Relays: {
        int start = it.type-1; int end = start + it.id; str = QObject::tr("第%１至%2 ").arg(start, end);
        if(it.value) str += QObject::tr("闭合"); else str += QObject::tr("断开"); break;}
    default: qDebug() << Q_FUNC_INFO; break;
    }

    sOpItem db;
    db.content = str;
    db.op_src =  QObject::tr("继电器设置");//opSrc(it.txType);
    Log_Core::bulid()->append(db);
}

bool Set_Output::outputCtrl(sDataItem &unit)
{
    bool ret = true; int id = unit.id; if(id) id--;
    sRelayUnit *it = &(cm::masterDev()->output.relay);
    if(it->en[id] || unit.txType == DTxType::TxWeb) {
        OP_Core::bulid()->relayCtrl(unit.id, unit.value);
    } else ret = false;

    return ret;
}

bool Set_Output::outputsCtrl(sDataItem &unit)
{
    bool ret = false; int start = unit.type-1; int end = start + unit.id;
    sRelayUnit *it = &(cm::masterDev()->output.relay);
    for(int i=start; i<end; ++i) {
        if(it->en[i] || unit.txType == DTxType::TxWeb) ret = true;
        else {ret = false; break;}
    }

    if(ret && unit.id) OP_Core::bulid()->relaysCtrl(start, end, unit.value);
    return ret;
}

bool Set_Output::groupCtrl(sDataItem &unit)
{
    QList<int> ids;
    bool ret = true;  int id = unit.id; if(id) {
        id--; ids = Data_Core::bulid()->outletByGroup(id);
    } else {
        for(int i=0; i<GROUP_NUM; ++i)
            ids << Data_Core::bulid()->outletByGroup(id);
    }

    sRelayUnit *it = &(cm::masterDev()->group.relay);
    if(it->en[id] || unit.txType == DTxType::TxWeb) {
        OP_Core::bulid()->relaysCtrl(ids, unit.value);
    } else ret = false;

    return ret;
}

bool Set_Output::relaySet(sDataItem &unit)
{
    bool ret = true;
    if(unit.addr) {
        ret = Cascade_Core::bulid()->masterSeting(unit);
    } else if(unit.rw) {
        if(unit.type == DType::Group) groupCtrl(unit); else {
            switch (unit.subtopic) {
            case DSub::Value:  ret = outputCtrl(unit); break;
            case DSub::Relays: ret = outputsCtrl(unit); break;
            case DSub::UpDelay: OP_Core::bulid()->setDelay(unit.id, unit.value); //break;
            default: ret = upMetaData(unit); Cfg_ReadWrite::bulid()->writeAlarms(); break;
            } } relayOpLog(unit);
    } else {ret = false; qDebug() << Q_FUNC_INFO;}

    return ret;
}


QString Set_Output::grouping(int addr, int id)
{
    QString res;
    QList<int> ids = Data_Core::bulid()->outletByGroup(id, addr);
    foreach(auto &i, ids) res += QString::number(i) +": ";
    return res;
}

QString Set_Output::groupName(int addr, int id)
{    
    sObjData *dev = &(cm::devData(addr)->group); if(id) id--;
    return dev->name[id];
}

QString Set_Output::outputName(int addr, int id)
{    
    sObjData *dev = &(cm::devData(addr)->output); if(id) id--;
    return dev->name[id];
}

QString Set_Output::outputTiming(int addr, int id, int onOff)
{
    char *ptr = nullptr; if(id) id--;
    sRelayUnit *it = &(cm::devData(addr)->output.relay);
    if(onOff) ptr = it->timingOff[id];
    else ptr = it->timingOn[id];
    return ptr;
}

QString Set_Output::groupTiming(int addr, int id, int onOff)
{
    char *ptr = nullptr; if(id) id--;
    sRelayUnit *it = &(cm::devData(addr)->group.relay);
    if(onOff) ptr = it->timingOff[id];
    else ptr = it->timingOn[id];
    return ptr;
}

void Set_Output::opNameLog(const sCfgItem &it, const QVariant &v)
{    
    QString str = QObject::tr("全部"); QString op;
    if(it.fc) str = QObject::tr("第%１").arg(it.fc);
    str += QObject::tr("名称修改为:%1").arg(v.toString());

    if(it.type == SFnCode::EGroupName) op = QObject::tr("组名称");
    else op += QObject::tr("输出位名称");

    sOpItem db;
    db.content = str;
    db.op_src = op; //opSrc(it.txType);
    Log_Core::bulid()->append(db);
}

bool Set_Output::outputNameSet(sCfgItem &it, const QVariant &v)
{
    bool ret = true;
    if(it.fc) {
        writeOpName(0, it.fc, v);
    } else {
        sObjData *obj = &(cm::masterDev()->output);
        for(int i=0; i<obj->size; ++i) writeOpName(0, i+1, v);
    } opNameLog(it, v);
    return ret;
}

bool Set_Output::groupNameSet(sCfgItem &it, const QVariant &v)
{
    bool ret = true;
    if(it.fc) {
        writeOpName(1, it.fc, v);
    } else {
        sObjData *obj = &(cm::masterDev()->group);
        for(int i=0; i<obj->size; ++i) writeOpName(1, i+1, v);
    } opNameLog(it, v);
    return ret;
}

bool Set_Output::groupingSet(sCfgItem &it, const QVariant &v)
{
    QStringList strs = v.toString().split("; ");
    sDevData *dev = cm::devData(it.addr);
    uchar *ptr = dev->cfg.nums.group[it.fc];
    memset(ptr, 0, OUTPUT_NUM);
    foreach(auto &str, strs) {
        int id = str.toInt(); ptr[id] = 1;
    } bool ret = false;
    if(strs.size()) ret = Cfg_ReadWrite::bulid()->writeParams();
    return ret;
}

bool Set_Output::setTiming(int g, int addr, int id, int onOff, const QVariant &v)
{
    char *ptr = nullptr; bool ret = true;
    sRelayUnit *it = &(cm::devData(addr)->output.relay);
    if(g) it = &(cm::devData(addr)->group.relay);
    if(onOff) ptr = it->timingOff[id];
    else ptr = it->timingOn[id];
    qstrcpy(ptr, v.toByteArray().data());
    if(ptr) Cfg_ReadWrite::bulid()->writeAlarms(); else ret = false;
    return ret;
}

bool Set_Output::setOutputTiming(int addr, int id, int onOff, const QVariant &v)
{
    bool ret = true; if(id) {
        ret = setTiming(0, addr, id, onOff, v);
    } else {
        sRelayUnit *it = &(cm::devData(addr)->output.relay);
        for(int i=0; i<it->size; ++i)
            ret = setTiming(0, addr, i, onOff, v);
    }

    return ret;
}

bool Set_Output::setGroupTiming(int addr, int id, int onOff, const QVariant &v)
{
    bool ret = true; if(id) {
        ret = setTiming(1, addr, id, onOff, v);
    } else {
        sRelayUnit *it = &(cm::devData(addr)->group.relay);
        for(int i=0; i<it->size; ++i)
            ret = setTiming(1, addr, i, onOff, v);
    }

    return ret;
}

void Set_Output::writeOpName(int fc, int id, const QVariant &name)
{
    QString prefix = "OutputName";
    if(fc) prefix = "GroupName";

    QString key = QString::number(id);
    Cfg_Obj *cfg = Cfg_Obj::bulid();
    cfg->writeCfg(key, name, prefix);

    sObjData *it = &(cm::masterDev()->output);
    if(fc) it = &(cm::masterDev()->group);
    qstrcpy((char *)it->name[id-1], name.toByteArray().data());
}
