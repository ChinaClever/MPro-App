/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_ntp.h"

sNtpCfg App_Ntp::ntpCfg;
App_Ntp::App_Ntp(QObject *parent)
    : App_NetAddr{parent}
{
    mUdp = new Net_Udp(this);
    QTimer::singleShot(55,this,SLOT(ntp_initSlot()));
    connect(mUdp, &Net_Udp::recvSig, this, &App_Ntp::ntp_recvSlot);
}

void App_Ntp::ntp_initSlot()
{
    if(ntpCfg.udp_en) mUdp->bind(123);
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

QString App_Ntp::ntp_timeZone()
{
    QTimeZone zone = QTimeZone::systemTimeZone();
    return QLocale::countryToString(zone.country());
}

bool App_Ntp::ntp_time(const QString &t)
{
    bool ret = true;
    QDateTime dt = QDateTime::fromString(t, "yyyy-MM-dd hh:mm:ss");
    QDateTime localDate = QDateTime::fromString("2022-09-01 10:40:00", "yyyy-MM-dd hh:mm:ss");
    if(dt > localDate) {
        QString qstrDateTime = QString("date -s %1").arg(t);
        system(qstrDateTime.toStdString().c_str());
        system("hwclock -w");
    }else ret = false;
    return ret;
}

bool App_Ntp::ntpdate()
{
    QString cmd = QString("ntpdate %1").arg(ntpCfg.ntp_host);
    return system(cmd.toStdString().c_str());
}
