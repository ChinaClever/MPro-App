/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "op_objrtu.h"


OP_ObjRtu::OP_ObjRtu(QObject *parent)
    : OP_Updater{parent}
{

}


uint OP_ObjRtu::readSn(uchar addr)
{
    uchar buf[] = {0x01, 0x03, 0xA0, 0x01, 0x00, 0x02, 0xB7, 0xCB};
    buf[0] = addr; ushort crc = Crc::rtu_crc(buf, 6);
    buf[6] = (0xff&crc);  buf[7]= (crc >> 8);
    QByteArray rcv = transmit(buf, 8); uint ret = 0;
    if(rcv.size() > 6) {
        for(int i=0; i<4; ++i)  ret = ret*256 + rcv.at(4+i);
    }

    return ret;
}

uchar *OP_ObjRtu::toInt(uchar *ptr, int line, uint *value)
{
    for(int i=0; i<line; ++i) {
        value[i] =  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取电能高8位
        value[i] <<= 16; // 左移8位
        value[i] +=  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取电能底8位
    }

    return ptr;
}

uchar *OP_ObjRtu::toOutputEle(uchar *ptr, int line, uint *value)
{
    for(int i=0; i<line; ++i) {
        value[i] = (*ptr++) << 16;
        value[i] += (*ptr++) << 8;
        value[i] += *ptr++;
    }

    return ptr;
}

uchar *OP_ObjRtu::toShort(uchar *ptr, int line, uint *value)
{
    for(int i=0; i<line; ++i) {
        value[i] =  (*ptr) * 256 + *(ptr+1);  ptr += 2;
    }

    return ptr;
}

uchar *OP_ObjRtu::toChar(uchar *ptr, int line, uint *value)
{
    for(int i=0; i<line; ++i) {
        value[i] = *(ptr++); // 读取电压
    }

    return ptr;
}


ushort OP_ObjRtu::getShort(uchar *ptr)
{
    return  (*ptr) * 256 + *(ptr+1);
}

uchar *OP_ObjRtu::getShort(uchar *ptr, int line, uchar *value)
{
    for(int i=0; i<line; ++i) {
        value[i] = getShort(ptr); ptr += 2;
    }

    return  ptr;
}
