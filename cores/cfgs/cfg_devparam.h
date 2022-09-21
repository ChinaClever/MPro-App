#ifndef CFG_DEVPARAM_H
#define CFG_DEVPARAM_H
#include "cfg_alarminitial.h"

class Cfg_devParam : public Cfg_AlarmInitial
{
public:
    Cfg_devParam();

    bool writeParams();
    bool initialParam();

protected:
    void initDevInfo(sDevNums &it);
    bool readParam(const QString &fn);

private:
    void uutInfoRead(sUutInfo &uut);
    void devNumRead(sDevNums &it);

private:
    Cfg_Obj *mDevCfg;
};

#endif // CFG_DEVPARAM_H
