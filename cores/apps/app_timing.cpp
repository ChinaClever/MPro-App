/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_timing.h"
#include "set_core.h"

App_Timing::App_Timing(QObject *parent)
    : App_Smtp{parent}
{
    QTimer::singleShot(755,this,SLOT(timing_initFunSlot()));
}

void App_Timing::timing_initFunSlot()
{
    mTimer = new QTimer(this);
    mTimer->start(60*1000); cm::masterDev()->cfg.param.runTime = 0;
    connect(mTimer, SIGNAL(timeout()), this, SLOT(timing_onTimeoutDone()));
}

void App_Timing::writeTotalTime(uint h)
{
    bool ret = false;
    if(h < 24*30) ret = true;
    else if(h < 24*30*2) {
        if(!h%3) ret = true;
    } else if(h < 24*30*3) {
        if(!h%6) ret = true;
    } else if(h < 24*30*4) {
        if(!h%12) ret = true;
    } if(!h%24) ret = true;

    if(ret) Cfg_ReadWrite::bulid()->writeParams();
}

void App_Timing::timing_onTimeoutDone()
{
    sParameter *param = &(cm::masterDev()->cfg.param);
    if(0 == (++param->runTime)%60) {
        uint h = param->totalTime++;
        if(h) writeTotalTime(h);
    }
}
