#ifndef APP_RUNING_H
#define APP_RUNING_H

#include "app_smtp.h"

class App_Runing : public App_Smtp
{
    Q_OBJECT
public:
    explicit App_Runing(QObject *parent = nullptr);

private slots:
    void runing_initFunSlot();
    void runing_onTimeoutDone();

private:
    QTimer *mTimer;
};

#endif // APP_RUNING_H
