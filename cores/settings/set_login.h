#ifndef SET_LOGIN_H
#define SET_LOGIN_H
#include "set_info.h"

class Set_Login : public Set_Info
{
public:
    Set_Login();
    QString loginUsrPwd(int type, int id=0);
    bool loginSet(uchar type, char *str, uchar txType, int id=0);
};

#endif // SET_LOGIN_H