#include "log_core.h"

Log_Core::Log_Core(QObject *parent)
    : QObject{parent}
{
    isRun = false;
    mThread = new CThread(this);
    //mThread->init(this, SLOT(run()));
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
    mUser = Db_User::bulid();
}



void Log_Core::writeLogs()
{
    if(!isRun) {
        isRun = true;
            QTimer::singleShot(100,this, SLOT(run()));
    //    QTimer::singleShot(100,mThread, SLOT(onceRun()));
    }

    saveLogs();
    qDebug() << "GGGGGGGG" << "DD" << isRun << mOpIts.size();
}


void Log_Core::saveLogs()
{
    Db_Tran t;
    qDebug() << "GGGGGGGG" << "DD" << isRun << mOpIts.size();
    while(mOpIts.size()) {
         qDebug() << "GGGGGGGG" << mOp->insertItem(mOpIts.takeFirst());
    }
    while(mUserIts.size()) mUser->insertItem(mUserIts.takeFirst());

    isRun = false;
}
