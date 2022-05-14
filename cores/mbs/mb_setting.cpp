/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mb_setting.h"
#include  "op_zrtu.h"

Mb_Setting::Mb_Setting(QObject *parent) : Mb_Env{parent}
{
    connect(this, &Modbus_SlaveObj::registerDataSig, this, &Mb_Setting::registerRecvSlot);
}

void Mb_Setting::mbSetUpdate()
{
    upSetData();
}

void Mb_Setting::upSetData()
{
    vshort vs;
    vs << mDevData->info.modbusAddr;
    vs << mDevData->info.buzzerSw;
    vs << mDevData->info.drySw;
    setRegs(MbReg_Settings, vs);
}

void Mb_Setting::addrSet(ushort &v)
{
    mDevData->info.modbusAddr = v;
    setAddress(v);
    qDebug() << Q_FUNC_INFO << "modbus set addr OK" << v;
}

void Mb_Setting::buzzerSw(ushort &v)
{
    mDevData->info.buzzerSw = v;
    qDebug() << Q_FUNC_INFO << "modbus set Buzzer switch OK" << v;
}

void Mb_Setting::drySw(ushort &v)
{
    mDevData->info.drySw = v;
    qDebug() << Q_FUNC_INFO << "modbus set dry switch OK" << v;
}

void Mb_Setting::startSet(ushort addr, ushort &value)
{
    switch (addr) {
    case MbReg_SetAddr: addrSet(value); break;
    case MbReg_SetBuzzer: buzzerSw(value); break;
    case MbReg_SetDry: drySw(value); break;
    }
}

void Mb_Setting::setAlarmUnit(sAlarmUnit *unit, int offset, ushort value)
{
    int id = offset / 2;
    if(offset % 2) unit->min[id] = value;
    else unit->max[id] = value;
}

void Mb_Setting::lineCurSet(ushort addr, ushort &value)
{
    if(addr < 1005) {
        switch (addr) {
        case 1003: mDevData->tg.cur.max = value; break;
        case 1004: mDevData->tg.cur.min = value; break;
        }
    } else {
        int offset = addr - 1005;
        sAlarmUnit *unit = &(mDevData->line.cur);
        setAlarmUnit(unit, offset, value*10);
    }
}

void Mb_Setting::outputCurSet(ushort addr, ushort &value)
{
    int offset = addr - 1011;
    sAlarmUnit *unit = &(mDevData->output.cur);
    setAlarmUnit(unit, offset, value*10);
}

void Mb_Setting::envSet(ushort addr, ushort &value)
{
    int offset = addr - 1107;
    sAlarmUnit *unit = nullptr;
    if(addr < 1123) unit = &(mDevData->env.tem);
    else if(addr < 1139) { offset = addr - 1123; unit = &(mDevData->env.hum);}
    if(unit) setAlarmUnit(unit, offset, value*10);
}

void Mb_Setting::lineVolSet(ushort addr, ushort &value)
{
    if(addr < 1143) {
        switch (addr) {
        case 1141: mDevData->tg.vol.max = value; break;
        case 1142: mDevData->tg.vol.min = value; break;
        }
    } else {
        int offset = addr - 1143;
        sAlarmUnit *unit = &(mDevData->line.vol);
        setAlarmUnit(unit, offset, value);
    }
}

void Mb_Setting::lineCrCurSet(ushort addr, ushort &value)
{
    if(addr < 1150) {
        mDevData->tg.cur.crMax = value;
    } else {
        int id = addr - 1150;
        sAlarmUnit *unit = &(mDevData->line.cur);
        unit->crMax[id] = value;
    }
}

void Mb_Setting::outputCrCurSet(ushort addr, ushort &value)
{
    int id = addr - 1153;
    sAlarmUnit *unit = &(mDevData->output.cur);
    unit->crMax[id] = value;
}

void Mb_Setting::powerOnDelay(ushort &value)
{
    OP_Core::bulid()->setDelay(0, value);
}

void Mb_Setting::loopCurMaxSet(ushort addr, ushort &value)
{
    int id = addr - 1202;
    sAlarmUnit *unit = &(mDevData->loop.cur);
    unit->max[id] = value;
}

void Mb_Setting::loopCurCrMaxSet(ushort addr, ushort &value)
{
    int id = addr - 1214;
    sAlarmUnit *unit = &(mDevData->loop.cur);
    unit->crMax[id] = value;
}

void Mb_Setting::loopCurMinSet(ushort addr, ushort &value)
{
    int id = addr - 1226;
    sAlarmUnit *unit = &(mDevData->loop.cur);
    unit->min[id] = value;
}

void Mb_Setting::timeSet(ushort addr, ushort &value)
{
    static uint t = 0;
    if(addr%2) {
        t = value << 16;
    } else {
        t += value;

        ////////////===========  时间同步
    }
}

void Mb_Setting::timeZone(ushort addr, ushort &value)
{
    static uint z = 0;
    if(addr%2) {
        z = value << 16;
    } else {
        z += value;

        ////////////===========  时区同步
    }
}

void Mb_Setting::otherSet(ushort addr, ushort &value)
{
    switch (addr) {
    case 1239: case 1240: timeSet(addr, value); break;
    case 1241: case 1242: timeZone(addr, value); break;
    }
}

void Mb_Setting::relaySet(ushort addr, ushort &value)
{
    if((addr >= 3000) && value) {
        uchar id = addr - 3000;
        OP_Core::bulid()->relayCtrl(id+1, value);
        //outputRelayCtrl(0, id+1, value);
    }
}

void Mb_Setting::eleClear(ushort addr)
{
    int id = addr - 4001;
    OP_Core::bulid()->clearEle(id);
}

void Mb_Setting::restoreFactoryDefaults()
{

}

void Mb_Setting::registerRecvSlot(int address, ushort value)
{
    if(address < 1003) startSet(address, value);
    else if(address < 1011) lineCurSet(address, value);
    else if(address < 1107) outputCurSet(address, value);
    else if(address < 1141) envSet(address, value);
    else if(address < 1149) lineVolSet(address, value);
    else if(address < 1153) lineCrCurSet(address, value);
    else if(address < 1201) outputCrCurSet(address, value);
    else if(address == 1201) powerOnDelay(value);
    else if(address < 1214) loopCurMaxSet(address, value);
    else if(address < 1226) loopCurCrMaxSet(address, value);
    else if(address < 1238) loopCurMinSet(address, value);
    else if(address < 1243) otherSet(address, value);
    else if(address < 3050) relaySet(address, value);
    else if(address == 4000) restoreFactoryDefaults();
    else if(address < 4050) eleClear(value);
    if(address < 2000) Set_readWrite::bulid()->writeSettings();
}
