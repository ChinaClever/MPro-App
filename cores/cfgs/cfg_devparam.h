#ifndef CFG_DEVPARAM_H
#define CFG_DEVPARAM_H
#include "cfg_alarminitial.h"

class Cfg_devParam : public Cfg_AlarmInitial
{
public:
    Cfg_devParam();

    void groupWrite();
    void totalTimeWrite(uint h);
    void devParamWrite(const QString &key, const QVariant &v, const QString &g);

protected:
    void initialParam();

private:
    void restartCntWrite();
    void devNumRead(sDevNums &it);
    void uutInfoRead(sUutInfo &uut);
    void devParamRead(sParameter &it);

private:
    Cfg_Obj *mDevCfg;
};

#endif // CFG_DEVPARAM_H
