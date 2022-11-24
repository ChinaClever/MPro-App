#ifndef SET_LOGIN_H
#define SET_LOGIN_H
#include "set_info.h"

class Set_Login : public Set_Info
{
public:
    Set_Login();
    QVariant loginUsrPwd(int type, int id);
    bool loginSet(uchar type, const QVariant &v, int id);

private:
    bool loginAuth(const QStringList &ls);
    bool loginCheck(const QString &str);
};

#endif // SET_LOGIN_H
