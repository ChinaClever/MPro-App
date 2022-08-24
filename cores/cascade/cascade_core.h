#ifndef CASCADE_CORE_H
#define CASCADE_CORE_H

#include "cascade_slave.h"

class Cascade_Core : public Cascade_Slave
{
    Q_OBJECT
    explicit Cascade_Core(QObject *parent = nullptr);
public:
    static Cascade_Core *bulid(QObject *parent = nullptr);
    ~Cascade_Core(){isRun = false;}
    void startFun();
    void stopFun();

public slots:
    void run();
    void initFunSlot();
    void initThreadSlot(){mThread->init(this, SLOT(run()));}

private:
    void workFun();

private:
    bool isRun=false;
    CThread *mThread;
};

#endif // CASCADE_CORE_H
