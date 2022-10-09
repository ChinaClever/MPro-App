#ifndef APP_LED_H
#define APP_LED_H

#include "app_buzzer.h"

class App_Led : public App_Buzzer
{
    Q_OBJECT
public:
    explicit App_Led(QObject *parent = nullptr);
    ~App_Led();

private:
    void led_delayOff();
    void led_workDown();

private slots:
    void led_initFunSlot();
    void led_run();

private:
    int mRgb[3]={0,0,0};
    bool mLedIsRun=false;
};

#endif // APP_LED_H
