#ifndef COMMONS_H
#define COMMONS_H
#include "datapacket.h"
#include "crc.h"

namespace cm {
void mdelay(int msec=1);

bool pingNet(const QString& ip);
bool isDigitStr(const QString &src);
bool isIPaddress(const QString& ip);
QString byteArrayToHexStr(const QByteArray &array);
QString byteArrayToUcharStr(const QByteArray &array);

bool language();
sDevData *masterDev();
sDataPacket *dataPacket();
sDevData *devData(uchar id);
QString execute(const QString &cmd);


const QDateTime buildDateTime();
template<typename T> QByteArray toByteArray(const T &v) {
    return QByteArray::fromRawData((char *)&v, sizeof(T));
}

template<typename T> T toStruct(const QByteArray &array) {
    T ret; memcpy(&ret, array.data(), sizeof(T)); return ret;
}



#define setbit(x,y) x|=(1<<y) //将X的第Y位置1
#define clrbit(x,y) x&=~(1<<y) //将X的第Y位清0
#define cout qDebug() << "[" << __FILE__ << ":" << Q_FUNC_INFO << ":" << __LINE__ << "]"
}

#endif // COMMONS_H
