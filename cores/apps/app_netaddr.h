#ifndef APP_NETADDR_H
#define APP_NETADDR_H
#include "app_sensorbox.h"

class App_NetAddr: public App_SensorBox
{
    Q_OBJECT
public:
    explicit App_NetAddr(QObject *parent = nullptr);
    void inet_setInterface();
    void inet_saveCfg(int fc);

private slots:
    void inet_initFunSlot();
    void inet_updateInterface();
    void inet_setInterfaceSlot();

private:
    void inet_setIpV4();
    void inet_setIpV6();
    void inet_dnsCfg();
    void inet_readCfg(sNetAddr &inet, const QString &g);
    void inet_writeCfg(sNetAddr &inet, const QString &g);

private:
    bool inet_isRun=false;
    Cfg_Obj *mInetCfg;
};

#endif // APP_NETADDR_H
