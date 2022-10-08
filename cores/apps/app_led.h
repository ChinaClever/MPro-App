#ifndef APP_LED_H
#define APP_LED_H

#include "app_buzzer.h"

class App_Led : public App_Buzzer
{
    Q_OBJECT
public:
    explicit App_Led(QObject *parent = nullptr);

signals:

};

#endif // APP_LED_H
