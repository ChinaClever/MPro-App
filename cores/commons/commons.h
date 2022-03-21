#ifndef COMMONS_H
#define COMMONS_H
#include "datapacket.h"

namespace cm {
void mdelay(int msec=1);

uchar xorNum(uchar *buf, int len);
uchar xorNum(const QByteArray &array);
void appendXorNum(QByteArray &array);

ushort CRC16(uchar *ptr, int len);
ushort CRC16(const QByteArray &array);

//ushort calccrc(ushort crc, uchar crcbuf);
ushort rtu_crc(uchar *buf, int len);
ushort rtu_crc(const QByteArray &array);

bool isDigitStr(const QString &src);
bool isIPaddress(const QString& ip);

QString byteArrayToHexStr(const QByteArray &array);
QString byteArrayToUcharStr(const QByteArray &array);

sDevData *masterDev();
sDataPacket *dataPacket();
sDevData *devData(uchar id);

const QDateTime buildDateTime();
template<typename T> QByteArray toByteArray(const T &v) {
    return QByteArray::fromRawData((char *)&v, sizeof(T));
}

template<typename T> T toStruct(const QByteArray &array) {
    T ret; memcpy(&ret, array.data(), sizeof(T)); return ret;
}

}







#endif // COMMONS_H
