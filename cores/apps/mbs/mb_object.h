#ifndef MB_OBJECT_H
#define MB_OBJECT_H

#include "mb_regs.h"

class Mb_Object : public Modbus_SlaveRtu
{
    Q_OBJECT
public:
    explicit Mb_Object(QObject *parent = nullptr);    
    void initFucRegs();

signals:

protected:
    bool setReg(ushort reg, const char *str);
    ushort getShort(uchar h, uchar l) {return ((h<<8) +l);}
    bool setReg(ushort reg, uchar h, uchar l) {return setRegister(reg, getShort(h, l));}

private:
    vshort strToShort(const char *str);
};

#endif // MB_OBJECT_H
