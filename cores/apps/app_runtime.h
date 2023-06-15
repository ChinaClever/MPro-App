#ifndef APP_RUNTIME_H
#define APP_RUNTIME_H

#include "app_ssh.h"

class App_RunTime : public App_Ssh
{
public:
    explicit App_RunTime(QObject *parent = nullptr);
    void runing_initFunSlot();
    void runing_onTimeoutDone();
    void runing_cpuDone();

private:
    QTimer *mTimer;
};

#endif // APP_RUNTIME_H
