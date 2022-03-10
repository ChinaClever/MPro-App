#include "op_object.h"

OP_Object::OP_Object(QObject *parent) : SerialPort{parent}
{

}

uchar OP_Object::xorNum(uchar *buf, int len)
{
    uchar xorsum = 0x00;
    for(int i=0; i<len; i++)
        xorsum ^= buf[i];
    return xorsum;
}

uchar OP_Object::xorNum(const QByteArray &array)
{
    uchar xorsum = 0x00;
    foreach(auto &it, array)
        xorsum ^= it;
     return xorsum;
}
