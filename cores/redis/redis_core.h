#ifndef REDIS_CORE_H
#define REDIS_CORE_H

#include "redis_obj.h"

class Redis_Core : public Redis_Obj
{
    explicit Redis_Core(QObject *parent = nullptr);
public:
    static Redis_Core *bulid(QObject *parent = nullptr);

private:
    void run();
    void workDown();

private:
    bool isRun=true;
};

#endif // REDIS_CORE_H
