#ifndef APP_KEY_H
#define APP_KEY_H

#include "app_buzzer.h"

class App_Key : public App_Buzzer
{
public:
    explicit App_Key(QObject *parent = nullptr);

public:
    void key_read();

private:
    int mKeyCnt=0;
};

#endif // APP_KEY_H
