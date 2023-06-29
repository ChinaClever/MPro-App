#ifndef AIOT_CORE_H
#define AIOT_CORE_H
#include "aiot_devcfg.h"

class Aiot_Core : public Aiot_DevCfg
{
    Aiot_Core();
public:
    static Aiot_Core *bulid();
    bool event_post(const QVariant& value);
    void setting();
    void run();

private:
    void workDown();

private:
    sAiotCfg *mCfg;
    bool isRun = true;
};

#endif // AIOT_CORE_H
