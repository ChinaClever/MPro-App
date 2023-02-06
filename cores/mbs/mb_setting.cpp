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
    vs << Mb_Core::modbusCfg.addr;
    vs << mDevData->cfg.param.buzzerSw;

    qint64 timestamp = QDateTime::currentSecsSinceEpoch();
    vs << timestamp/0xffff; vs << timestamp%0xffff;
    vs << mDevData->cfg.param.drySw;
    setRegs(MbReg_Setting+1, vs);
}

void Mb_Setting::addrSet(ushort &v)
{
    Cfg_Com::bulid()->writeCfg("addr", v, "modbus");
    Mb_Core::modbusCfg.addr = v; setAddress(v);
    mDevData->cfg.param.modbusRtuAddr = v;
    cout << "modbus set addr OK" << v;
}

void Mb_Setting::buzzerSw(ushort &v)
{
    mDevData->cfg.param.buzzerSw = v;
    cout << "modbus set Buzzer switch OK" << v;
    Cfg_Core::bulid()->devParamWrite("buzzerSw", v, "devParams");
}

void Mb_Setting::drySw(ushort &v)
{
    mDevData->cfg.param.drySw = v;
    cout << "modbus set dry switch OK" << v;
    Cfg_Core::bulid()->devParamWrite("buzzerSw", v, "devParams");
}

void Mb_Setting::startSet(ushort addr, ushort &value)
{
    switch (addr) {
    case MbReg_SetAddr: addrSet(value); break;
    case MbReg_SetBuzzer: buzzerSw(value); break;
    case MbReg_SetDry: drySw(value); break;
    case MbReg_SetEle: OP_Core::bulid()->clearEle(0); break;
    case MbReg_SetTime: case MbReg_SetTime+1: timeSet(addr, value); break;
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


void Mb_Setting::restoreFactoryDefaults()
{

}

void Mb_Setting::registerRecvSlot(int address, ushort value)
{
    if(address < 6100) startSet(address, value);
    else if(address < 6150) line_setting(address, value);
    else if(address < 6300) loop_setting(address, value);
    else if(address < 6900) output_setting(address, value);
    else if(address < 7000) output_ctrl(address, value);
    else if(address < 7050) env_setting(address, value);
    else if(address < 7160) group_setting(address, value);
    else if(address == 8001) restoreFactoryDefaults();
    if(address > MbReg_Setting) Set_Core::bulid()->writeAlarm();
}
