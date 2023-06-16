#ifndef APP_LED_H
#define APP_LED_H

#include "app_key.h"

class App_Led : public App_Key
{
public:
    explicit App_Led(QObject *parent = nullptr);
    ~App_Led();
    void led_run();

private:
    void led_delayOff();
    void led_workDown();
    void led_initFun();
    void led_factoryRestore();

private:
    bool m_fr = true;
    int mRgb[3]={0,0,0};
    bool mLedIsRun=false;
};

#endif // APP_LED_H
