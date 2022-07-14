#ifndef MB_OBJECT_H
#define MB_OBJECT_H

#include "mb_regs.h"

class Mb_Object : public Modbus_SlaveRtu , public Set_Output
{
    Q_OBJECT
public:
    explicit Mb_Object(QObject *parent = nullptr);

protected:
    void upDevInfo();
    bool setReg(ushort reg, const char *str);
    bool setReg(ushort reg, ushort v) {return setRegister(reg, v);}
    bool setRegs(ushort reg, const vshort &v) {return setRegisters(reg, v);}
    bool setReg2(ushort reg, uchar h, uchar l) {return setRegister(reg, getShort(h, l));}

private:
    void initFucRegs();
    vshort strToShort(const char *str);
    ushort getShort(uchar h, uchar l) {return ((h<<8) +l);}

protected:
    sDevData *mDevData;
};

#endif // MB_OBJECT_H
