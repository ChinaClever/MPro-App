#ifndef APP_NETADDR_H
#define APP_NETADDR_H
#include "commons.h"

class App_NetAddr: public QObject
{
    Q_OBJECT
    explicit App_NetAddr(QObject *parent = nullptr);
public:
    static App_NetAddr *bulid(QObject *parent = nullptr);
    void setInterface();

public slots:
    void updateInterface();

private slots:
    void setInterfaceSlot();

private:
    void initFun();
    void setIpV4();
    void dhcp(const QString &n);

private:
    bool isRun=false;
};

#endif // APP_NETADDR_H
