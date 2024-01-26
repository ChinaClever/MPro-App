#ifndef APP_NETADDR_H
#define APP_NETADDR_H
#include "app_sensorbox.h"

class App_NetAddr: public App_SensorBox
{
public:
    explicit App_NetAddr(QObject *parent = nullptr);
    void inet_setInterface();
    void inet_saveCfg(int fc, const sNetInterface &net);

    void inet_initFunSlot();
    void inet_updateInterface();
    void inet_setInterfaceSlot();
    void inet_readCfg(sNetAddr &inet, const QString &g);

private:
    void inet_setIpV4();
    void inet_setIpV6();
    void inet_dnsCfg();
    int inet_dhcpUpdate();
    void inet_writeCfg(const sNetAddr &inet, const QString &g);
    QString getIpv4GatewayAddress();
    QString getIpv6GatewayAddress();

private:
    int mCnt = 1;
    bool inet_isRun=false;
    Cfg_Obj *mInetCfg;
};

#endif // APP_NETADDR_H
