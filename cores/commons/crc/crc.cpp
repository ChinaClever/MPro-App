#include "crc.h"
extern "C" {
#include "libcrc/crc.h"
}

#define INITIAL_CRC_CC3 0x1D0F
#define CRC_CCITT_POLY 0x1021

ushort Crc::CRC16(uchar *ptr, int len) // AUG-CCITT
{
    ushort crc = INITIAL_CRC_CC3;
    while (len-- > 0)
    {
        crc = crc ^ ((uint16_t) (*ptr++ << 8));  // --len;
        for (int i = 0; i < 8; i++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ CRC_CCITT_POLY;
            } else {
                crc = crc << 1;
            }
        }
    }

    return crc;
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
    return crc8_cal(pdata, len);
}

ushort Crc::Cal16(uchar *pdata, uint len)
{
    return crc16_cal(pdata, len);
}

uint Crc::Cal32(uchar *pdata, uint len)
{
    return crc32_cal(pdata, len);
}

uint Crc::Cal32(const QByteArray &array)
{
    return Cal32((uchar *)array.data(), array.size());
}

ushort Crc::Cal(uchar *puchMsg, uint len)
{
    return Cal16(puchMsg, len);
}

ushort Crc::Cal(const QByteArray &array)
{
    return Cal((uchar *)array.data(), array.size());
}

uint Crc::File(const QString &fn)
{
    uint ret = CRC32_INIT_VAL;
    int max = 1024; QFile file(fn);
    if(file.exists() && file.open(QIODevice::ReadOnly)) {
        while (!file.atEnd() && ret) {
            QByteArray data = file.read(max);
            uchar *ptr = (uchar *)data.data();
            ret = crc32_cyc_cal(ret, ptr, data.size());
        } file.close();
    }

    return (ret ^ CRC32_INIT_VAL);
}
