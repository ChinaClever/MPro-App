#ifndef CRC_H
#define CRC_H
#include <QtCore>

namespace Crc {
uchar XorNum(uchar *buf, int len);
uchar XorNum(const QByteArray &array);
void XorNumAppend(QByteArray &array);

ushort Checksum(const char *ptr, int len);
ushort Checksum(const QByteArray &array);
void ChecksumAppend(QByteArray &array);

ushort Rtu(uchar *buf, int len);
ushort Rtu(const QByteArray &array);
void RtuAppend(QByteArray &array);
uint16_t CRC_16(uint8_t *puchMsg, uint16_t usDataLen);

}



#endif // CRC_H
