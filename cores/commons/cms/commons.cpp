/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "commons.h"
#ifdef Q_OS_LINUX
#include "shm.h"
#endif

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

QString cm::execute(const QString &cmd)
{
    QProcess pro;
    pro.start(cmd);
    pro.waitForFinished();
    QByteArray bs = pro.readAllStandardOutput();
    bs +=  pro.readAllStandardError();
    return QString::fromLocal8Bit(bs);
    //QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
}

QString cm::executes(const QStringList &cmds)
{
    QProcess pro;
    foreach (const auto &cmd, cmds) {
        pro.start(cmd);
        pro.waitForFinished();
    }
    QByteArray bs = pro.readAllStandardOutput();
    bs +=  pro.readAllStandardError();
    return QString::fromLocal8Bit(bs);
}

bool cm::pingNet(const QString& ip)
{   
    bool bPingSuccess = false;
    QString strArg = "ping -c3 " + ip;  //strPingIP 为设备IP地址
    QString p_stdout = execute(strArg);
    if(p_stdout.contains("ttl=")) { //我采用这个字符来判断 对不对？
        bPingSuccess = true;
    }else {
        bPingSuccess = false;
    }

    return bPingSuccess;
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

bool cm::language()
{
    return masterDev()->cfg.param.language ? false: true;
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
#ifdef Q_OS_LINUX
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

uint cm::runTime(uchar id)
{
    sDevData *dev = devData(id);
    return dev->proc.core.runSec;
}

sDevData *cm::masterDev()
{
    return devData(0);
}
#endif
const QDateTime cm::buildDateTime()
{
   QString dateTime;
   dateTime += __DATE__; dateTime += __TIME__;
   dateTime.replace("  "," 0");//注意" "是两个空格，用于日期为单数时需要转成“空格+0”
   return QLocale(QLocale::English).toDateTime(dateTime, "MMM dd yyyyhh:mm:ss");
}

void cm::buildDateTime(char *ptr)
{
    QString str = buildDateTime().toString("yyyy-MM-dd hh:mm:ss");
    qstrcpy(ptr, str.toUtf8().data());
}


double cm::decimal(const sDataItem &it)
{
    double res = 1; switch (it.topic) {
    case DTopic::Vol: res = COM_RATE_VOL; break;
    case DTopic::Cur: res = COM_RATE_CUR; break;
    case DTopic::Pow: res = COM_RATE_POW; break;
    case DTopic::Ele: res = COM_RATE_ELE; break;
    case DTopic::PF: res = COM_RATE_PF; break;
    case DTopic::ArtPow: res = COM_RATE_POW; break;
    case DTopic::ReactivePow: res = COM_RATE_POW; break;
    case DTopic::Tem: res = COM_RATE_TEM; break;
    case DTopic::Hum: res = COM_RATE_HUM; break;
    //default: cout << it.topic; break;
    }

    if((DSub::Size==it.subtopic) || (DSub::Alarm==it.subtopic) || (DSub::EnAlarm==it.subtopic)) res = 1;

    return res;
}
