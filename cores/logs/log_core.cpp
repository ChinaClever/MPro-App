#include "log_core.h"

Log_Core::Log_Core(QObject *parent)
    : QObject{parent}
{
    isRun = false;
    QTimer::singleShot(65,this,SLOT(initFunSlot()));
}


Log_Core *Log_Core::bulid(QObject *parent)
{
    static Log_Core *sington = nullptr;
    if(!sington) sington = new Log_Core(parent);
    return sington;
}

void Log_Core::initFunSlot()
{
    mOp = Db_Op::bulid();
    mSys = Db_Sys::bulid();
    mUser = Db_User::bulid();    
    mAlarm = Db_Alarm::bulid();
}

void Log_Core::run()
{
    if(!isRun) {isRun = true; QTimer::singleShot(350,this, SLOT(saveLogSlot())); }
}

void Log_Core::saveLogSlot()
{
    Db_Tran t;
    while(mOpIts.size()) mOp->insertItem(mOpIts.takeFirst());
    while(mSysIts.size()) mSys->insertItem(mSysIts.takeFirst());
    while(mUserIts.size()) mUser->insertItem(mUserIts.takeFirst());
    while(mAlarmIts.size()) mAlarm->insertItem(mAlarmIts.takeFirst());

    isRun = false;
}
