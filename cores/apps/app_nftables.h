#ifndef APP_NFTABLES_H
#define APP_NFTABLES_H

#include "app_netaddr.h"

class App_Nftables : public App_NetAddr
{
    Q_OBJECT
public:
    explicit App_Nftables(QObject *parent = nullptr);

private slots:
    void nft_initSlot();
};

#endif // APP_NFTABLES_H
