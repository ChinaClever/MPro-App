/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "log_core.h"
#include "commons.h"

Log_Core::Log_Core(QObject *parent)
    : Log_Read{parent}
{
    uint t = 6*60*60*1000;
    timer = new QTimer(this);
    timer->start(t + rand()%100);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    QTimer::singleShot(65,this,SLOT(initFunSlot()));
}


Log_Core *Log_Core::bulid(QObject *parent)
{
    static Log_Core *sington = nullptr;
    if(!sington) {
        sington = new Log_Core(parent);
        int cnt = cm::masterDev()->cfg.param.restartCnt;

        sSysItem it;
        it.module = "system";
        it.content = "Software startup ";
        it.content += QString::number(cnt);
        sington->append(it);
    }
    return sington;
}

void Log_Core::initFunSlot()
{
    mOp = Db_Op::bulid();
    mOta = Db_Ota::bulid();
    mSys = Db_Sys::bulid();
    mUser = Db_User::bulid();    
    mAlarm = Db_Alarm::bulid();
    mHardware = Db_Hardware::bulid();
}

void Log_Core::run()
{
    if(!isRun) {isRun = true; QTimer::singleShot(350,this, SLOT(saveLogSlot()));}
}

void Log_Core::saveLogSlot()
{
    Db_Tran t;
    while(mOpIts.size()) mOp->insertItem(mOpIts.takeFirst());
    while(mOtaIts.size()) mOta->insertItem(mOtaIts.takeFirst());
    while(mEleIts.size()) mEle->insertItem(mEleIts.takeFirst());
    while(mSysIts.size()) mSys->insertItem(mSysIts.takeFirst());
    while(mUserIts.size()) mUser->insertItem(mUserIts.takeFirst());
    while(mAlarmIts.size()) mAlarm->insertItem(mAlarmIts.takeFirst());
    while(mHardwareIts.size()) mHardware->insertItem(mHardwareIts.takeFirst());
    isRun = false;
}

void Log_Core::timeoutDone()
{
    Db_Tran t;
    int cnt = 10000;
    mOp->countsRemove(cnt);
    mSys->countsRemove(cnt);
    mUser->countsRemove(cnt);
    mAlarm->countsRemove(cnt);
    mEle->countsRemove(10*cnt);
}
