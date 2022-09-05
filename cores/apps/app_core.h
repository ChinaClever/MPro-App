#ifndef APP_CORE_H
#define APP_CORE_H

#include "app_start.h"

class App_Core : public App_Start
{
    Q_OBJECT
    explicit App_Core(QObject *parent = nullptr);
public:
    static App_Core *bulid(QObject *parent = nullptr);

};

#endif // APP_CORE_H
