/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_ntp.h"

sNtpCfg App_Ntp::ntpCfg;
App_Ntp::App_Ntp(QObject *parent)
    : App_Shell{parent}
{
    mUdp = new Net_Udp(this);
    QTimer::singleShot(556,this,&App_Ntp::ntp_initSlot);
    connect(mUdp, &Net_Udp::recvSig, this, &App_Ntp::ntp_recvSlot);
}

void App_Ntp::ntp_initSlot()
{
#if (QT_VERSION < QT_VERSION_CHECK(5,15,0))
    if(ntpCfg.udp_en) mUdp->bind(123);
    ntp_timeZone(ntpCfg.time_zone);
#endif
}

bool App_Ntp::ntp_udpEn(bool en)
{
    if(en) mUdp->bind(123);
    ntpCfg.udp_en = en;
    return en;
}

QString App_Ntp::ntp_time()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    return dateTime.toString("yyyy-MM-dd hh:mm:ss");
}

void App_Ntp::ntp_timeZone(const QString &zone)
{
    ntpCfg.time_zone = zone;
    system("rm -rf /usr/data/etc/localtime");
    QString cmd = "ln -s /usr/share/zoneinfo/%1";
    cmd += " /usr/data/etc/localtime";
    system(cmd.arg(zone).toLocal8Bit().data());
    //qDebug() << cmd.arg(zone);
}

bool App_Ntp::ntp_time(const QString &t)
{
    bool ret = true;
    QDateTime dt = QDateTime::fromString(t, "yyyy-MM-dd hh:mm:ss");
    QDateTime localDate = QDateTime::fromString("2022-09-01 10:40:00", "yyyy-MM-dd hh:mm:ss");
    if(dt > localDate) {
        QString qstrDateTime = QString("date -s '%1'").arg(t); qDebug() << qstrDateTime;
        system(qstrDateTime.toStdString().c_str()); system("hwclock -w -u"); system("sync");
    }else ret = false;
    return ret;
}

bool App_Ntp::ntpdate()
{
    bool ret = true;
    if(ntpCfg.ntp_host.size()) {
        QString cmd = QString("ntpdate %1").arg(ntpCfg.ntp_host);
        qDebug() << cmd; system(cmd.toStdString().c_str());
    } else ret = false;
    return ret;
}
