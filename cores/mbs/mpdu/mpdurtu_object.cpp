/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mpdurtu_object.h"

Mpdurtu_Object::Mpdurtu_Object(QObject *parent) : Modbus_SlaveRtu{parent}
{
    mDevData = cm::masterDev();
}

bool Mpdurtu_Object::setReg(ushort reg, const char *str)
{
    vshort values = strToShort(str);
    return setHoldingRegisters(reg, values);
}

vshort Mpdurtu_Object::strToShort(const char *str)
{
    vshort res; ushort buf[32] = {0};
    int size = strlen(str) + 1;
    memcpy(buf, str, size);
    uchar *ptr = (uchar *)str;

    for(int i=0; i<size; i+=2) {
        buf[i] = (ptr[i]<<8) + ptr[i+1];
        res.append(buf[i]);
    }

    return res;
}


bool Mpdurtu_Object::alarmUnitCheck(int reg, int id, sAlarmUnit *unit, ushort v)
{
    bool ret = true; switch (reg) {
    case 1: if((v > unit->rated[id]*1.3) || (v < unit->crMax[id])) ret = false; break;
    case 2: if((v > unit->max[id]) || (v < unit->crMin[id])) ret = false; break;
    case 3: if((v > unit->crMax[id]) || (v < unit->min[id])) ret = false; break;
    case 4: if(v > unit->crMin[id]) ret = false; break;
    } if(id >= unit->size) ret = false;

    return ret;
}

int Mpdurtu_Object::setting(sDataItem &it, uint value)
{
    it.txType = DTxType::TxModbus; // 通讯类型 1 Web
    it.value = value; it.rw = 1; // 0 读  1 写
    return Set_Core::bulid()->setting(it);
}

int Mpdurtu_Object::setCfg(sCfgItem &it, const QVariant &v)
{
    it.txType = DTxType::TxModbus;
    return Set_Core::bulid()->setCfg(it, v);
}

void Mpdurtu_Object::upDevInfo()
{
    sDevData *dev = mDevData;
    vshort vs;
    sDataPacket *data = cm::dataPacket();

    vs << dev->cfg.param.devSpec; // 设备规格
    QString strip = data->net.inet.ip;
    QStringList ip = strip.split(".");
    QString strmac = data->net.mac;
    QStringList mac = strmac.split(":");
    if(strip.isEmpty()){
        vs << 0 << 0 << 0 << 0;
    }else{
        vs << ip.at(0).toUInt();
        vs << ip.at(1).toUInt();
        vs << ip.at(2).toUInt();
        vs << ip.at(3).toUInt();
    }
    if(strmac.isEmpty()){
        vs << 0 << 0 << 0 << 0 << 0 << 0;
    }else{
        bool ok;
        vs << mac.at(0).toUInt(&ok,16);
        vs << mac.at(1).toUInt(&ok,16);
        vs << mac.at(2).toUInt(&ok,16);
        vs << mac.at(3).toUInt(&ok,16);
        vs << mac.at(4).toUInt(&ok,16);
        vs << mac.at(5).toUInt(&ok,16);
    }
    vs << dev->cfg.nums.outputNum;

    setRegs(MReg_Type, vs);
}
