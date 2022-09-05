#ifndef APP_NTP_H
#define APP_NTP_H
#include "net_udp.h"
#include "app_netaddr.h"

struct sNtpCfg {
    sNtpCfg(){udp_en=0; }
    int udp_en;
    QString ntp_host;
    QString time_zone;
};

class App_Ntp : public App_NetAddr
{
    Q_OBJECT
public:
    explicit App_Ntp(QObject *parent = nullptr);
    static sNtpCfg ntpCfg;

    bool ntp_time(const QString &t);
    QString ntp_time();
    QString ntp_timeZone();
    bool ntpdate();
    bool ntp_udpEn(bool en);

private slots:
    void ntp_initSlot();
    void ntp_recvSlot(const QByteArray &array){if(ntpCfg.udp_en) ntp_time(array);}

private:
    Net_Udp *mUdp;
};

#endif // APP_NTP_H
