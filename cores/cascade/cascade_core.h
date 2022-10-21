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
    void run();

private:
    void workFun();
    void initFunSlot();

private:
    bool isRun=true;
};

#endif // CASCADE_CORE_H
