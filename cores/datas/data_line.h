#ifndef DATA_LINE_H
#define DATA_LINE_H
#include "data_loop.h"

class Data_Line : public Data_Loop
{
public:
    Data_Line();

protected:
    void lineWork();
    void tgWork();

private:
    void calHz();
    void inletNum();
};

#endif // DATA_LINE_H
