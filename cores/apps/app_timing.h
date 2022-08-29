#ifndef APP_TIMING_H
#define APP_TIMING_H

#include "app_netaddr.h"

class App_Timing : public QObject
{
    Q_OBJECT
    explicit App_Timing(QObject *parent = nullptr);
public:
    static App_Timing *bulid(QObject *parent = nullptr);

private:
    void writeTotalTime(uint h);

private slots:
    void initFunSlot();
    void onTimeoutDone();

private:
    QTimer *mTimer;
};

#endif // APP_TIMING_H
