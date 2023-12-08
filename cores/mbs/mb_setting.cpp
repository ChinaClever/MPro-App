/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mb_setting.h"
#include "app_core.h"
#include "mb_core.h"
#include "cfg_core.h"


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
    vs << Mb_Core::modbusCfg.addrRtu;
    vs << mDevData->cfg.param.buzzerSw;
    setRegs(mStartReg+MbReg_Setting+1, vs);

    //qint64 timestamp = QDateTime::currentSecsSinceEpoch();
    //vs << timestamp/0xffff; vs << timestamp%0xffff;
    //vs << mDevData->cfg.param.drySw;
    //setRegs(MbReg_SetTime+1, vs);
}


void Mb_Setting::addrSet(ushort &v)
{
    if(mDevData->cfg.param.modbusRtuAddr != v) {
        Cfg_Com::bulid()->writeCfg("addr", v, "modbus");
        Mb_Core::modbusCfg.addrRtu = v; setAddress(v);
        mDevData->cfg.param.modbusRtuAddr = v;
    } cout << "modbus set addr OK" << v;
}

void Mb_Setting::buzzerSw(uchar addr, ushort &v)
{
    sDevData *dev = cm::devData(addr);
    if(dev->cfg.param.buzzerSw != v) {
        sCfgItem it;
        it.addr = addr;
        it.type = SFnCode::EModbus;
        it.fc = 7; setCfg(it, v);
    }

    //mDevData->cfg.param.buzzerSw = v;
    //cout << "modbus set Buzzer switch OK" << v;
    //Cfg_Core::bulid()->devParamWrite("buzzerSw", v, "devParams");
}

void Mb_Setting::drySw(ushort &v)
{
    mDevData->cfg.param.drySw = v;
    cout << "modbus set dry switch OK" << v;
    Cfg_Core::bulid()->devParamWrite("buzzerSw", v, "devParams");
}

void Mb_Setting::startSet(ushort addr, ushort reg, ushort &value)
{
    switch (reg) {
    case MbReg_SetAddr: addrSet(value); break;
    case MbReg_SetBuzzer: buzzerSw(addr, value); break;
    case MbReg_SetDry: drySw(value); break;
    case MbReg_SetEle: output_clearEle(addr, 0);  break;
     //case MbReg_SetTime: case MbReg_SetTime+1: timeSet(addr, value); break;
    }
}


void Mb_Setting::timeSet(ushort addr, ushort &value)
{
    static uint t = 0;
    if(addr%2) {
        t = value << 16;
    } else {
        t += value;
        QDateTime dt = QDateTime::fromTime_t(t);
        QString str = dt.toString("yyyy-MM-dd hh:mm:ss");
        App_Core::bulid()->ntp_time(str); //cout << str;
    }
}

void Mb_Setting::restoreFactoryDefaults(ushort addr)
{
    sCfgItem it;
    it.addr = addr;
    it.type = SFnCode::ESys;
    it.fc = 1; setCfg(it, 1);
}

void Mb_Setting::registerRecvSlot(int address, ushort value)
{
    cout << address << value;

    ushort reg = address % MbReg_MaxSize;
    ushort addr = address / MbReg_MaxSize;
    if(reg < 3100) startSet(addr, reg, value);
    else if(reg < 3150) line_setting(addr, reg, value);
    else if(reg < 3300) loop_setting(addr, reg, value);
    else if(reg < 3900) output_setting(addr, reg, value);
    else if(reg < 4000) output_ctrl(addr, reg, value);
    else if(reg < 4050) env_setting(addr, reg, value);
    else if(reg < 4300) group_setting(addr, reg, value);
    else if(reg < 4325) output_dualCtrl(addr, reg, value);
    else if(reg == MbReg_MaxSize-1) restoreFactoryDefaults(addr);
    //if(address > MbReg_Setting) Set_Core::bulid()->writeAlarm();
}
