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
    void stopFun(){isRun = false;}
    void startFun();

public slots:
    void run();

private:
    bool isRun=false;
    bool isInit=false;
    CThread *mThread;
};

#endif // CASCADE_CORE_H
