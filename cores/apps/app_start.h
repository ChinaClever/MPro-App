#ifndef APP_START_H
#define APP_START_H

#include "app_runtime.h"

class App_Start : public App_RunTime
{
public:
    explicit App_Start(QObject *parent = nullptr);
    ~App_Start();

    void initSysSlot();
    void initFunSlot();
    void startThreadSlot();
};

#endif // APP_START_H
