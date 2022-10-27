#ifndef APP_NFTABLES_H
#define APP_NFTABLES_H

#include "app_netaddr.h"

class App_Nftables : public App_NetAddr
{
public:
    explicit App_Nftables(QObject *parent = nullptr);

private:
    void nft_initSlot();
};

#endif // APP_NFTABLES_H
