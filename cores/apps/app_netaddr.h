#ifndef APP_NETADDR_H
#define APP_NETADDR_H
#include "commons.h"

class App_NetAddr: public QObject
{
    Q_OBJECT
public:
    explicit App_NetAddr(QObject *parent = nullptr);
    void inet_setInterface();

public slots:
    void inet_updateInterface();

private slots:
    void inet_initFunSlot();
    void inet_setInterfaceSlot();

private:
    void inet_setIpV4();
    void inet_dhcp(const QString &n);

private:
    bool inet_isRun=false;
};

#endif // APP_NETADDR_H
