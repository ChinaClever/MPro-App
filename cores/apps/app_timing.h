#ifndef APP_TIMING_H
#define APP_TIMING_H

#include "app_smtp.h"

class App_Timing : public App_Smtp
{
    Q_OBJECT
public:
    explicit App_Timing(QObject *parent = nullptr);

private:
    void writeTotalTime(uint h);

private slots:
    void timing_initFunSlot();
    void timing_onTimeoutDone();

private:
    QTimer *mTimer;
};

#endif // APP_TIMING_H
