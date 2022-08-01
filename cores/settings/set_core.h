#ifndef SET_CORE_H
#define SET_CORE_H
#include "set_integr.h"

class Set_Core : public Set_Integr
{
    Set_Core();
public:
    static Set_Core *bulid();
    void writeAlarm();
    bool setting(sDataItem &it);
    bool setCfg(sCfgItem &it);
    QString getCfg(sCfgItem &it);

private:
    bool setNumber(sCfgItem &it);
    int getNumber(sCfgItem &it);
    bool setString(sCfgItem &it);
    QString getString(sCfgItem &it);
};

#endif // SET_CORE_H
