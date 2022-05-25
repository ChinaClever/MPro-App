#ifndef SET_CORE_H
#define SET_CORE_H
#include "set_output.h"

class Set_Core : public Set_Output
{
    Set_Core();
public:
    static Set_Core *bulid();
    void writeSettings();
    bool setting(sDataItem &it);
    bool setString(sStrItem &it);
    QString getString(sStrItem &it);
};

#endif // SET_CORE_H
