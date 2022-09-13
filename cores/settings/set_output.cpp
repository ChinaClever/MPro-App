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
    QString str = QStringLiteral("全部");
    if(it.id) str = QStringLiteral("第%１ ").arg(it.id);
    switch (it.subtopic) {
    case DSub::Value:
        if(it.type == DType::Group) str += QStringLiteral("组开关 ");
        else if(it.type == DType::Dual) str += QStringLiteral("双电源开关 ");
        else str += QStringLiteral("输出位继电器 ");
        if(it.value) str += QStringLiteral("闭合"); else str += QStringLiteral("断开");
        break;
    case DSub::Rated:
        str += QStringLiteral("输出位继电器模式切换 ");
        if(sRelay::OffALarm == it.value) str += QStringLiteral("断开报警模式");
        else {str += QStringLiteral("默认");} break;
    case DSub::UpDelay: str += QStringLiteral("输出位继电器上电延时，修改为 %1s").arg(it.value); break;
    case DSub::ResetDelay: str += QStringLiteral("输出位继电器复位延时，修改为 %1s").arg(it.value); break;
    case DSub::OverrunOff: str += QStringLiteral("输出位超限断电，修改为 %1s").arg(it.value); break;
    case DSub::TimingEn: str += QStringLiteral("输出位定时功能，修改为 %1s").arg(it.value); break;
    case DSub::Relays: {
        int start = it.type-1; int end = start + it.id; str = QStringLiteral("第%１至%2 ").arg(start, end);
        if(it.value) str += QStringLiteral("闭合"); else str += QStringLiteral("断开"); break;}
    default: qDebug() << Q_FUNC_INFO; break;
    }

    sOpItem db;
    db.content = str;
    db.op_src =  QStringLiteral("继电器设置");//opSrc(it.txType);
    Log_Core::bulid()->append(db);
}

bool Set_Output::outputCtrl(sDataItem &unit)
{
    bool ret = true; int id = unit.id; if(id) id--;
    sRelayUnit *it = &(cm::masterDev()->output.relay);
    if(unit.type == DType::Dual) it = &(cm::masterDev()->dual.relay);
    if(it->en[id] || unit.txType == DTxType::TxWeb) {
        OP_Core::bulid()->relayCtrl(unit.id, unit.value);
    } else ret = false;

    return ret;
}

bool Set_Output::outputsCtrl(sDataItem &unit)
{
    sRelayUnit *it = &(cm::masterDev()->output.relay);
    bool ret = false; int start = unit.type-1; int end = start + unit.id;
    if(unit.type == DType::Dual) it = &(cm::masterDev()->dual.relay);
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
        ids = Data_Core::bulid()->outletByGroup(id-1);
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
    if(unit.rw) {
        if(unit.type == DType::Group) groupCtrl(unit);
        else {
            switch (unit.subtopic) {
            case DSub::Value:  ret = outputCtrl(unit); break;
            case DSub::Relays: ret = outputsCtrl(unit); break;
            case DSub::UpDelay: OP_Core::bulid()->setDelay(unit.id, unit.value); //break;
            default: ret = upMetaData(unit); Cfg_ReadWrite::bulid()->writeAlarms(); break;
            } } relayOpLog(unit);
    } else if(unit.type == DType::Dual) {
        unit.addr = 1; ret = Cascade_Core::bulid()->masterSeting(unit);
    } else {ret = false; qDebug() << Q_FUNC_INFO;}

    return ret;
}

QString Set_Output::outputCfg(sCfgItem &it)
{
    sDevData *dev = cm::devData(it.addr);
    QString res; int id = it.sub; if(id) id--;
    sObjData *obj = nullptr;

    switch (it.type) {
    case SFnCode::EOutput: obj = &(dev->output); break;
    case SFnCode::EGroup: obj = &(dev->group); break;
    case SFnCode::EDual: obj = &(dev->dual); break;
    default: qDebug() << Q_FUNC_INFO << it.type; return res;
    }

    switch (it.fc) {
    case 1: res = obj->name[id]; break;
    case 2: res = obj->relay.timingOn[id]; break;
    case 3: res = obj->relay.timingOff[id]; break;
    default: qDebug() << Q_FUNC_INFO << it.fc; break;
    }
    return res;
}


QString Set_Output::grouping(int addr, int id)
{
    QString res;
    QList<int> ids = Data_Core::bulid()->outletByGroup(id, addr);
    foreach(auto &i, ids) res += QString::number(i) +": ";
    return res;
}


QString Set_Output::outputName(int addr, int id)
{    
    sCfgItem it; it.fc = 1; it.addr = addr;
    it.type = SFnCode::EOutput; it.sub = id;
    return outputCfg(it);
}

void Set_Output::opNameLog(const sCfgItem &it, const QVariant &v)
{    
    QString str = QStringLiteral("全部"); QString op;
    if(it.fc) str = QStringLiteral("第%１").arg(it.fc);
    str += QStringLiteral("名称修改为:%1").arg(v.toString());

    switch (it.type) {
    case SFnCode::EOutput: op += QStringLiteral("输出位名称"); break;
    case SFnCode::EGroup: op += QStringLiteral("组名称"); break;
    case SFnCode::EDual: op += QStringLiteral("机架名称"); break;
    default: qDebug() << Q_FUNC_INFO; break;
    }

    sOpItem db;
    db.content = str;
    db.op_src = op; //opSrc(it.txType);
    Log_Core::bulid()->append(db);
}

bool Set_Output::outputNameSet(sCfgItem &it, const QVariant &v)
{
    it.type = SFnCode::EOutput;
    it.sub = it.fc; it.fc = 1;
    return outputSet(it, v);
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

bool Set_Output::outputSetById(sCfgItem &it, const QVariant &v)
{
    QString prefix; bool res=true; int id = it.sub;
    sObjData *obj = nullptr; char *ptr = nullptr;
    sDevData *dev = cm::devData(it.addr);

    switch (it.type) {
    case SFnCode::EOutput: obj = &(dev->output); prefix = "OutputName"; break;
    case SFnCode::EGroup: obj = &(dev->group); prefix = "GroupName"; break;
    case SFnCode::EDual: obj = &(dev->dual); prefix = "DualName"; break;
    default: res = false; qDebug() << Q_FUNC_INFO << it.type; return res;
    }

    switch (it.fc) {
    case 1: ptr = obj->name[id]; break;
    case 2: ptr = obj->relay.timingOn[id]; break;
    case 3: ptr = obj->relay.timingOff[id]; break;
    default: res = false; qDebug() << Q_FUNC_INFO << it.fc; break;
    }

    if(ptr){
        qstrcpy(ptr, v.toByteArray().data());
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
    int id = it.sub;
    bool ret = false; if(it.sub){
        it.sub--; ret = outputSetById(it, v);
    } else {
        sObjData *obj = nullptr;
        sDevData *dev = cm::devData(it.addr);
        switch (it.type) {
        case SFnCode::EOutput: obj = &(dev->output); break;
        case SFnCode::EGroup: obj = &(dev->group); break;
        case SFnCode::EDual: obj = &(dev->dual); break;
        default: qDebug() << Q_FUNC_INFO << it.type; return ret;
        }
        for(int i=0; i<obj->size; ++i) {
            it.sub = i; ret = outputSetById(it, v);
        }
    }

    if(it.fc == 1){it.sub = id; opNameLog(it, v); }
    else Cfg_ReadWrite::bulid()->writeAlarms();
    if(it.type == DType::Dual) {
        it.addr = 1; ret = Cascade_Core::bulid()->masterSetCfg(it, v);
    }

    return ret;
}

