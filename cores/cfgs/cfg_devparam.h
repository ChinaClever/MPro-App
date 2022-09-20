#ifndef CFG_DEVPARAM_H
#define CFG_DEVPARAM_H
#include "cfg_alarminitial.h"

class Cfg_devParam
{
public:
    Cfg_devParam();

    bool initialParam();

private:
    void initDevInfo(sDevNums &it);
};

#endif // CFG_DEVPARAM_H
