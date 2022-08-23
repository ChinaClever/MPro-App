#ifndef SET_CORE_H
#define SET_CORE_H
#include "set_netaddr.h"

class Set_Core : public Set_NetAddr
{
    Set_Core();
public:
    static Set_Core *bulid();
    QVariant getCfg(sCfgItem &it);
    bool setting(sDataItem &it);
    bool setCfg(sCfgItem &it, const QVariant &v);
    void writeAlarm();

private:
    bool setParam(sCfgItem &it, const QVariant &v);
};

#endif // SET_CORE_H
