#include "cthread.h"

void mdelay(int msec)
{
    if (msec <= 0) {
        return;
    }

#if 1
    //非阻塞方式延时,现在很多人推荐的方法
    QEventLoop loop;
    QTimer::singleShot(msec, &loop, SLOT(quit()));
    loop.exec();
#else
#if (QT_VERSION >= QT_VERSION_CHECK(5,0,0))
    //阻塞方式延时,如果在主线程会卡住主界面
    QThread::msleep(msec);
#else
    //非阻塞方式延时,不会卡住主界面,据说可能有问题
    QTime endTime = QTime::currentTime().addMSecs(msec);
    while (QTime::currentTime() < endTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
#endif
#endif
}


CThread::CThread(QObject *parent)
    : QObject{parent}
{
    mThread = new QThread(parent);
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
        if(isSleep) mdelay(1);
    }
}
