/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_timing.h"
#include "set_core.h"

App_Timing::App_Timing(QObject *parent)
    : QObject{parent}
{
    QTimer::singleShot(755,this,SLOT(initFunSlot()));
}

App_Timing *App_Timing::bulid(QObject *parent)
{
    static App_Timing* sington = nullptr;
    if(sington == nullptr) {
        sington = new App_Timing(parent);
    }
    return sington;
}

void App_Timing::initFunSlot()
{
    mTimer = new QTimer(this);
    mTimer->start(60*1000); cm::masterDev()->cfg.param.runTime = 0;
    connect(mTimer, SIGNAL(timeout()), this, SLOT(onTimeoutDone()));
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

void App_Timing::onTimeoutDone()
{
    sParameter *param = &(cm::masterDev()->cfg.param);
    if(0 == (++param->runTime)%60) {
        uint h = param->totalTime++;
        if(h) writeTotalTime(h);
    }
}
