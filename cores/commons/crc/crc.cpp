/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "crc.h"
#include "libcrc/CRC.h"

ushort Crc::CRC16(uchar *ptr, int len) // AUG-CCITT
{
    return CRC::Calculate(ptr, len, CRC::CRC_16_CCITTFALSE());
}

ushort Crc::CRC16(const QByteArray &array)
{
    return CRC16((uchar *)array.data(), array.size());
}

static ushort calccrc(ushort crc, uchar crcbuf)
{
    uchar x, kkk=0;
    crc = crc^crcbuf;
    for(x=0;x<8;x++)
    {
        kkk = crc&1;
        crc >>= 1;
        crc &= 0x7FFF;
        if(kkk == 1)
            crc = crc^0xa001;
        crc=crc&0xffff;
    }
    return crc;
}

ushort Crc::rtu_crc(uchar *buf, int len)
{
    ushort crc = 0xffff;
    for(int i=0; i<len; i++)
        crc = calccrc(crc, buf[i]);
    return crc;
}

ushort Crc::rtu_crc(const QByteArray &array)
{
    return rtu_crc((uchar *)array.data(), array.size());
}

uchar Crc::XorNum(uchar *buf, int len)
{
    uchar xorsum = 0x00;
    for(int i=0; i<len; i++)
        xorsum ^= buf[i];
    return xorsum;
}

uchar Crc::XorNum(const QByteArray &array)
{
    uchar xorsum = 0x00;
    foreach(auto &it, array)
        xorsum ^= it;
    return xorsum;
}

void Crc::AppendXorNum(QByteArray &array)
{
    uchar xorsum = XorNum(array);
    array.append(xorsum);
}


uchar Crc::Cal8(uchar *pdata, uint len)
{
    return CRC::Calculate(pdata, len, CRC::CRC_8());
}

ushort Crc::Cal16(uchar *pdata, uint len)
{
    return CRC::Calculate(pdata, len, CRC::CRC_16_ARC());
}

uint Crc::Cal32(uchar *pdata, uint len)
{
    return CRC::Calculate(pdata, len, CRC::CRC_32());
}

uint Crc::Cal32(const QByteArray &array)
{
    return Cal32((uchar *)array.data(), array.size());
}

uint Crc::Cal32(const QByteArray &array, uint crc)
{
    uchar *ptr = (uchar *)array.data();
    return CRC::Calculate(ptr, array.size(), CRC::CRC_32(), crc);
}

void Crc::AppendCrc(QByteArray &array)
{
    ushort crc = rtu_crc(array);
    array.append(0xff & crc);
    array.append(crc >> 8);
}

uint Crc::File(const QString &fn)
{
    uint ret = 0;
    int max = 1024; QFile file(fn);
    if(file.exists() && file.open(QIODevice::ReadOnly)) {
        while (!file.atEnd() && ret) {
            QByteArray data = file.read(max);
            if(ret) ret = Cal32(data, ret);
            else ret = Cal32(data);
        } file.close();
    }

    return ret;
}
