#ifndef DATA_CORE_H
#define DATA_CORE_H
#include "data_dual.h"

class Data_Core : public Data_Dual
{
    explicit Data_Core();
public:
    static Data_Core *bulid();
    ~Data_Core(){isRun = false;}
    void run();

private:
    void upAlarm();

private:
    bool isRun=true;
};

#endif // DATA_CORE_H
