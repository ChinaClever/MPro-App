#ifndef APP_CORE_H
#define APP_CORE_H

#include "app_whitelist.h"

class App_Core : public App_Start
{
    explicit App_Core(QObject *parent = nullptr);
public:
    static App_Core *bulid(QObject *parent = nullptr);

private:
    void initVer();
    void compileTime();
};

#endif // APP_CORE_H
