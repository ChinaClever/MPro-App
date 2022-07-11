/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "commons.h"
#include "shm.h"


void cm::mdelay(int msec)
{
    if (msec <= 0) {
        return;
    }

#if 1
    //非阻塞方式延时,现在很多人推荐的方法
    QEventLoop loop;
    QTimer::singleShot(msec, &loop, SLOT(quit()));
    loop.exec();
#else
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    //阻塞方式延时,如果在主线程会卡住主界面
    QThread::msleep(msec);
#else
    //非阻塞方式延时,不会卡住主界面,据说可能有问题
    QTime endTime = QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() < endTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
#endif
#endif
}

/***
  *判断一个字符串是否为纯数字
  */
bool cm::isDigitStr(const QString &src)
{
    QByteArray ba = src.toLatin1();//QString 转换为 char*
    const char *s = ba.data();

    while(*s && *s>='0' && *s<='9') s++;

    if (*s) //不是纯数字
        return false;

    return true;
}

/**
 * @brief 是否为IP地址
 * @param ip
 * @return
 */
bool cm::isIPaddress(const QString& ip)
{
    QRegExp regExp("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    if(regExp.exactMatch(ip))
        return true ;

    return false;
}


/**
 * @brief 转16进制
 * @param array
 * @return
 */
QString cm::byteArrayToHexStr(const QByteArray &array)
{
    QString strArray = array.toHex(); // 十六进制
    for(int i=0; i<array.size(); ++i)
        strArray.insert(2+3*i, " "); // 插入空格
    return strArray;
}

/**
 * @brief 转十进制
 * @param array
 * @return
 */
QString cm::byteArrayToUcharStr(const QByteArray &array)
{
    QString strArray ;
    for(int i=0; i<array.size(); ++i)
        strArray += QString::number((uchar)array.at(i)) + " ";  // 十进制
    return strArray;
}

sDataPacket *cm::dataPacket()
{
    sDataPacket *packet = (sDataPacket *)Shm::sharedMemory();
    if(!packet) qDebug() << "Error cm dataPacket";
    return packet;
}

sDevData *cm::devData(uchar id)
{
    return &(dataPacket()->data[id]);
}

sDevData *cm::masterDev()
{
    return devData(0);
}

const QDateTime cm::buildDateTime()
{
   QString dateTime;
   dateTime += __DATE__; dateTime += __TIME__;
   dateTime.replace("  "," 0");//注意" "是两个空格，用于日期为单数时需要转成“空格+0”
   return QLocale(QLocale::English).toDateTime(dateTime, "MMM dd yyyyhh:mm:ss");
}
