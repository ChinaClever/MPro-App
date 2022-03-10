#ifndef OP_OBJRTU_H
#define OP_OBJRTU_H

#include "op_updater.h"

class OP_ObjRtu : public OP_Updater
{
    Q_OBJECT
public:
    explicit OP_ObjRtu(QObject *parent = nullptr);
    virtual bool readData(int addr, sOpIt &it)=0;

protected:
    uchar *toInt(uchar *ptr, int line, uint *value);
    uchar *toChar(uchar *ptr, int line, uchar *value);
    uchar *toShort(uchar *ptr, int line, ushort *value);
    uchar *toOutputEle(uchar *ptr, int line, uint *value);
    uchar *getShort(uchar *ptr, int line, uchar *value);
    ushort getShort(uchar *ptr);
};

#endif // OP_OBJRTU_H
