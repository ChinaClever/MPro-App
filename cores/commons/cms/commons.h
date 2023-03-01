#ifndef COMMONS_H
#define COMMONS_H
#include "datapacket.h"
#ifdef CRC_H
#include "crc.h"
#endif

namespace cm {
void mdelay(int msec=1);

bool pingNet(const QString& ip);
bool isDigitStr(const QString &src);
bool isIPaddress(const QString& ip);
bool qrcodeGenerator(const QString& msg);
QByteArray zipCompress(const QByteArray &array);
QByteArray zipUncompress(const QByteArray &array);
QString byteArrayToHexStr(const QByteArray &array);
QString byteArrayToUcharStr(const QByteArray &array);

bool language();
sDevData *masterDev();
sDataPacket *dataPacket();
sDevData *devData(uchar id);
QString execute(const QString &cmd);
QString executes(const QStringList &cmds);
uint runTime(uchar id=0);

void buildDateTime(char *ptr);
const QDateTime buildDateTime();
template<typename T> QByteArray toByteArray(const T &v) {
    return QByteArray::fromRawData((char *)&v, sizeof(T));
}

template<typename T> T toStruct(const QByteArray &array) {
    T ret; memcpy(&ret, array.data(), sizeof(T)); return ret;
}

double decimal(const sDataItem &it);
#define setbit(x,y) x|=(1<<y) //将X的第Y位置1
#define clrbit(x,y) x&=~(1<<y) //将X的第Y位清0
#define cout qDebug() << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz") << "[" << __FILE__ << ":" << Q_FUNC_INFO << ":" << __LINE__ << "]"
}

#endif // COMMONS_H
