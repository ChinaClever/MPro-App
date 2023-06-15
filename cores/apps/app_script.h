#ifndef APP_SCRIPT_H
#define APP_SCRIPT_H

#include "app_netaddr.h"
#define SCRIPT_SIZE 20
struct sScriptCfg {
    uchar type[SCRIPT_SIZE]={0};
    uchar startup[SCRIPT_SIZE]={0};
    QString cmd[SCRIPT_SIZE];
    QStringList result[SCRIPT_SIZE];
};

class App_Script : public App_NetAddr
{
public:
    explicit App_Script(QObject *parent = nullptr);
    static sScriptCfg scriptCfg;
    void script_execute(int id);
    void script_kill(int id);

private:
    void script_initSlot();
    void script_readProcess();

private:
    //QMap<int, QProcess *> mMap;
    QTimer *mScriptTimer;
    QMap<int, FILE *> mMap;
};

#endif // APP_SCRIPT_H
