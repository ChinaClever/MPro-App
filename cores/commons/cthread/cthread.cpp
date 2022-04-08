#include "cthread.h"
#include "commons.h"

CThread::CThread(QObject *parent)
    : QObject{parent}
{
    mThread = new QThread(this);
    connect(mThread, &QThread::finished, this, &CThread::finished);
}

void CThread::start(bool run)
{
    isSleep = true;
    isContinue = run;
    if(!mThread->isRunning()) {
        mThread->start();
    }
}

void CThread::stop()
{
    isSleep = false;
    isContinue = false;
    mThread->quit();
    mThread->wait();
}

void CThread::init(QObject *p, const char *fun)
{
    connect(mThread, SIGNAL(started()), p, fun);
    p->moveToThread(mThread);
}

void CThread::finished()
{
    if(isContinue) mThread->start();
}

void CThread::msleep(int msec)
{
    for(int i=0; i<msec; ++i) {
        if(isSleep) cm::mdelay(1);
    }
}
