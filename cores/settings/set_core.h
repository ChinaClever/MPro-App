#ifndef SET_CORE_H
#define SET_CORE_H
#include "set_diagnose.h"

class Set_Core : public Set_Diagnose
{
    Set_Core();
public:
    static Set_Core *bulid();
    QVariant getCfg(sCfgItem &it);
    int setting(sDataItem &it);
    int setCfg(sCfgItem &it, const QVariant &v);
    void writeAlarm();

private:
    int setParam(sCfgItem &it, const QVariant &v);
};

#endif // SET_CORE_H
