#ifndef APP_WHITELIST_H
#define APP_WHITELIST_H

#include "app_start.h"
#define WHITE_LIST_NUM 4

struct sWhiteListCfg
{
    int en;
    QString ip[WHITE_LIST_NUM];
    QString mac[WHITE_LIST_NUM];
};

class App_WhiteList : public App_Start
{
public:
    explicit App_WhiteList(QObject *parent = nullptr);
    static sWhiteListCfg whiteListCfg;
    void internetFirewall();
};

#endif // APP_WHITELIST_H
