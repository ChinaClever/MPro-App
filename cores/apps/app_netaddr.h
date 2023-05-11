#ifndef APP_NETADDR_H
#define APP_NETADDR_H
#include "app_sensorbox.h"

class App_NetAddr: public App_SensorBox
{
public:
    explicit App_NetAddr(QObject *parent = nullptr);
    void inet_setInterface();
    void inet_saveCfg(int fc);

    void inet_initFunSlot();
    void inet_updateInterface();
    void inet_setInterfaceSlot();

private:
    void inet_setIpV4();
    void inet_setIpV6();
    void inet_dnsCfg();
    int inet_dhcpUpdate();
    void inet_readCfg(sNetAddr &inet, const QString &g);
    void inet_writeCfg(sNetAddr &inet, const QString &g);

private:
    int mCnt = 1;
    bool inet_isRun=false;
    Cfg_Obj *mInetCfg;
};

#endif // APP_NETADDR_H
