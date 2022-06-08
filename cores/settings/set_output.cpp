/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_output.h"
#include "op_core.h"
#include "cascade_core.h"

Set_Output::Set_Output()
{

}

void Set_Output::relayOpLog(const sDataItem &it)
{
    QString str = QObject::tr("全部");
    if(it.id) str = QObject::tr("第%１ ").arg(it.id);
    switch (it.subtopic) {
    case DSub::Value:
        str += QObject::tr("输出位继电器 ");
        if(it.value) str += QObject::tr("闭合"); else str += QObject::tr("断开");
        break;
    case DSub::Rated:
        str += QObject::tr("输出位继电器模式切换 ");
        if(sRelay::NormaOpen == it.value) str += QObject::tr("常闭合");
        else if(sRelay::NormaClose == it.value) str += QObject::tr("常断开");
        else {str += QObject::tr("默认");} break;
    case DSub::VMax: str += QObject::tr("输出位继电器上电延时，修改为 %1s").arg(it.value); break;
    default: qDebug() << Q_FUNC_INFO; break;
    }

    sOpItem db;
    db.content = str;
    db.op_src = opSrc(it.txType);
    Log_Core::bulid()->append(db);
}

bool Set_Output::relaySet(sDataItem &unit)
{
    bool ret = true;
    if(unit.addr) {
        ret = Cascade_Core::bulid()->masterSet(unit);
    } else if(unit.rw) {
        switch (unit.subtopic) {
        case DSub::Value: OP_Core::bulid()->relayCtrl(unit.id, unit.value); break;
        case DSub::VMax:  OP_Core::bulid()->setDelay(unit.id, unit.value); //break;
        default: ret = upIndexValue(unit); break;
        } Set_ReadWrite::bulid()->writeSettings();
        relayOpLog(unit);
    } else qDebug() << Q_FUNC_INFO;

    return ret;
}

QString Set_Output::outputName(int addr, int id)
{
    sObjData *dev = &(cm::devData(addr)->output);
    return dev->name[id];
}

void Set_Output::opNameLog(const sStrItem &it)
{
    QString str = QObject::tr("全部");
    if(it.id) str = QObject::tr("第%１").arg(it.id);
    str += QObject::tr("输出位名称修改为:%2").arg(it.str);

    sOpItem db;
    db.content = str;
    db.op_src = opSrc(it.txType);
    Log_Core::bulid()->append(db);
}

bool Set_Output::outputNameSet(sStrItem &it)
{
    bool ret = true;
    if(it.id) {
        writeOpName(it.id, it.str);
    } else {
        sObjData *obj = &(cm::masterDev()->output);
        for(int i=0; i<obj->size; ++i) writeOpName(i+1, it.str);
    } opNameLog(it);
    return ret;
}

void Set_Output::writeOpName(int id, const QString &name)
{
    QString prefix = "OutputName";
    QString key = QString::number(id);
    Cfg_Obj *cfg = Cfg_Obj::bulid(CFG_FN);
    cfg->writeCfg(key, name, prefix);

    sObjData *it = &(cm::masterDev()->output);
    qstrcpy((char *)it->name[id-1], name.toLatin1().data());
}
