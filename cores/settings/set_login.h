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
    int loginTryLock();
    void loginLocking(bool ok);
    bool loginAuth(const QStringList &ls);
    bool loginCheck(const QString &str);

private:
    int mFailCnt=0;
    QDateTime mDt;
};

#endif // SET_LOGIN_H
