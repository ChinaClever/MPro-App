#include "mb_object.h"

Mb_Object::Mb_Object(QObject *parent) : Modbus_SlaveRtu{parent}
{

}

void Mb_Object::initFucRegs()
{
    setReg(MbReg_Factory, "CLEVER");
    setReg(MbReg_Model, "HVPDU63-2-32(01)");
    setReg(MbReg_Hw, "A");

    setReg(MbReg_Sw, 1,1);
    setReg(MbReg_Version, 1,8);

    const char *t = "20210102";
    setReg(MbReg_CompileTime, t);
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
