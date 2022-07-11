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
    vshort vs; initFucRegs();
    vs << mDevData->tg.vol.rated; // 额定电压
    vs << mDevData->tg.cur.rated; // 额定电流
    vs << 50; // 额定频率

    vs << mDevData->info.outputNum;
    vs << 1 << 1 << 1;
    setRegs(MbReg_Rateds, vs);

    setReg(MbReg_PhaseNum, mDevData->info.lineNum);
    setReg(MbReg_Classify, 2);
    setReg(MbReg_Classify+1, 0xFF);
}

bool Mb_Object::setReg(ushort reg, const char *str)
{
    vshort values = strToShort(str);
    return setHoldingRegisters(reg, values);
}

vshort Mb_Object::strToShort(const char *str)
{
    vshort res; ushort buf[32] = {0};
    int size = (strlen(str) + 1) / 2;
    memcpy(buf, str, size);

    for(int i=0; i<size; ++i) {
        res.append(buf[i]);
    }

    return res;
}
