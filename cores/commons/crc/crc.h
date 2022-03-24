#ifndef CRC_H
#define CRC_H
#include <QtCore>

namespace Crc {
uchar XorNum(uchar *buf, int len);
uchar XorNum(const QByteArray &array);
void AppendXorNum(QByteArray &array);

uchar Cal8(uchar *pdata, uint len);
uint Cal32(uchar *pdata, uint len);
uint Cal32(const QByteArray &array);
ushort Cal16(uchar *pdata, uint len);

uint File(const QString &fn);

ushort Cal(uchar *puchMsg, uint len);
ushort Cal(const QByteArray &array);

ushort CRC16(uchar *ptr, int len);
ushort CRC16(const QByteArray &array);

ushort rtu_crc(uchar *buf, int len);
ushort rtu_crc(const QByteArray &array);
}



#endif // CRC_H
