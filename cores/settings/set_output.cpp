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

bool Set_Output::relaySet(sDataItem &unit)
{
    bool ret = true;
    if(unit.addr) {
        ret = Cascade_Core::bulid()->masterSet(unit);
    } else if(unit.rw) {
        switch (unit.subtopic) {
        case DSub::Value: OP_Core::bulid()->relayCtrl(unit.id, unit.value); break;
        case DSub::VMax:  OP_Core::bulid()->setDelay(unit.id, unit.value);break;
        default: ret = upIndexValue(unit); break;
        } Set_ReadWrite::bulid()->writeSettings();
    } else qDebug() << Q_FUNC_INFO;

    return ret;
}

bool Set_Output::outputRelayCtrl(int addr, int id, uchar on)
{
    sDataItem unit;
    unit.addr = addr;
    unit.type = DType::Output;
    unit.topic = DTopic::Relay;
    unit.subtopic = DSub::Value;

    unit.rw = 1;
    unit.id = id;
    unit.value = on;

    return relaySet(unit);
}

bool Set_Output::outputDelaySet(int addr, int id, uchar delay)
{
    sDataItem unit;
    unit.addr = addr;
    unit.type = DType::Output;
    unit.topic = DTopic::Relay;
    unit.subtopic = DSub::VMax; // 延时设置

    unit.rw = 1;
    unit.id = id;
    unit.value = delay;

    return relaySet(unit);
}


bool Set_Output::outputSwModeSet(int addr, int id, uchar mode)
{
    sDataItem unit;
    unit.addr = addr;
    unit.type = DType::Output;
    unit.topic = DTopic::Relay;
    unit.subtopic = DSub::Rated;

    unit.rw = 1;
    unit.id = id;
    unit.value = mode;

    return relaySet(unit);
}

QString Set_Output::outputName(int addr, int id)
{
    sObjData *dev = &(cm::devData(addr)->output);
    return dev->name[id-1];
}

bool Set_Output::outputNameSet(int addr, int id, const QString &name)
{
    bool ret = true;
    if(addr) {
        sStrItem item;
        item.addr = addr; item.id = id; item.fc = 10; item.rw = 1;
        qstrcpy((char *)item.str, name.toLatin1().data());
        ret = Cascade_Core::bulid()->masterSetString(item);
    } else {
        if(id) {
            writeOpName(id, name);
        } else {
            sObjData *it = &(cm::masterDev()->output);
            for(int i=0; i<it->size; ++i) writeOpName(i+1, name);
        }
    }
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
