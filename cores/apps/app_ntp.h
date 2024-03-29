#ifndef APP_NTP_H
#define APP_NTP_H
#include "net_udp.h"
#include "app_script.h"

struct sNtpCfg {
    sNtpCfg(){udp_en=0; }
    int udp_en;
    QString ntp_host;
    QString time_zone;
};

class App_Ntp : public App_Script
{
public:
    explicit App_Ntp(QObject *parent = nullptr);
    static sNtpCfg ntpCfg;

    void ntp_timeZone(const QString &zone);
    bool ntp_time(const QString &t);
    bool ntp_udpEn(bool en);
    QString ntp_time();
    bool ntpdate();

    void ntp_initSlot();
    void ntp_recvSlot(const QByteArray &array){if(ntpCfg.udp_en) ntp_time(array);}

private:
    Net_Udp *mUdp;
    QMap<QString, QString> mTzMap;
};

#endif // APP_NTP_H
