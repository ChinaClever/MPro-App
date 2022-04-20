#ifndef DATA_CORE_H
#define DATA_CORE_H
#include "data_line.h"

class Data_Core : public Data_Line
{
    explicit Data_Core();
public:
    static Data_Core *bulid();
    ~Data_Core(){isRun = false;}

protected:
    void run() override;

private:
    bool isRun=true;
};

#endif // DATA_CORE_H
