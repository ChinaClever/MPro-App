#ifndef DATA_ENV_H
#define DATA_ENV_H
#include "data_group.h"

class Data_Env : public Data_Group
{
public:
    Data_Env();

protected:
    void envWork();

private:
    void setEnvSize();
    void initData();
};

#endif // DATA_ENV_H
