#ifndef APP_SENSOR_H
#define APP_SENSOR_H

#include "app_led.h"

class App_Sensor : public App_Led
{
public:
    explicit App_Sensor(QObject *parent = nullptr);
    ~App_Sensor();

private:
    void env_workDown();
    void env_initFun();
    void env_run();

private:
    bool mEnvIsRun=false;
    int mFds[SENOR_NUM]={-1,-1};
};

#endif // APP_SENSOR_H
