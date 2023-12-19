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
