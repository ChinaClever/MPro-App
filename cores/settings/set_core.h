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
    bool setNumStr(sNumStrItem &it);
    QString getNumStr(sNumStrItem &it);

private:
    bool setNumber(sNumStrItem &it);
    int getNumber(sNumStrItem &it);
    bool setString(sNumStrItem &it);
    QString getString(sNumStrItem &it);
};

#endif // SET_CORE_H
