#ifndef APP_SHELL_H
#define APP_SHELL_H

#include "app_netaddr.h"
#define SHELL_SIZE 10
struct sShellCfg {
    uchar startup[SHELL_SIZE]={0};
    QString cmd[SHELL_SIZE];
    QStringList result[SHELL_SIZE];
};

class App_Shell : public App_NetAddr
{
public:
    explicit App_Shell(QObject *parent = nullptr);
    static sShellCfg shellCfg;
    void shell_execute(int id);

private:
    void shell_initSlot();
    void shell_readProcess();

private:
    QMap<int, QProcess *> mMap;
};

#endif // APP_SHELL_H
