#ifndef APP_RUNTIME_H
#define APP_RUNTIME_H

#include "app_smtp.h"

class App_RunTime : public App_Smtp
{
    Q_OBJECT
public:
    explicit App_RunTime(QObject *parent = nullptr);

private slots:
    void runing_initFunSlot();
    void runing_onTimeoutDone();

private:
    QTimer *mTimer;
};

#endif // APP_RUNTIME_H