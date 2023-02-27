/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mb_object.h"

Mb_Object::Mb_Object(QObject *parent) : Modbus_SlaveRtu{parent}
{
    mDevData = cm::masterDev();
}

void Mb_Object::initFucRegs()
{
    setReg(MbReg_Factory, "CLEVER");
    setReg(MbReg_Model, "HVPDU63-2-32(01)");
    setReg(MbReg_Hw, "A");
    setReg(MbReg_Sw, 100);
    setReg2(MbReg_Version, 1,9);

    QString t = cm::buildDateTime().toString("yyyyMMdd");
    char *str = t.toLocal8Bit().data();
    setReg(MbReg_CompileTime, str);
}

void Mb_Object::upDevInfo()
{
    sDevData *dev = mDevData;
    vshort vs; //initFucRegs();
    vs << 1 << 0; // 通讯协议版本

    vs << dev->cfg.param.devSpec; // 设备规格
    vs << dev->cfg.nums.lineNum;
    vs << dev->cfg.nums.loopNum;
    vs << dev->cfg.nums.outputNum;
    vs << dev->cfg.nums.boardNum;
    vs << dev->hz;

    //qint64 timestamp = QDateTime::currentSecsSinceEpoch();
    qint64 timestamp = QDateTime::currentDateTime().toSecsSinceEpoch();
    vs << (timestamp>> 16); vs << (timestamp&0xffff);

    vs << dev->status;
    vs << dev->tg.pow.value;
    vs << dev->tg.artPow;
    vs << dev->tg.pf;
    vs << (dev->tg.ele >> 16);
    vs << (dev->tg.ele & 0xffff);

    setRegs(0, vs);
}

bool Mb_Object::setReg(ushort reg, const char *str)
{
    vshort values = strToShort(str);
    return setHoldingRegisters(reg, values);
}

vshort Mb_Object::strToShort(const char *str)
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
