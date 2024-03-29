#ifndef SET_LOGIN_H
#define SET_LOGIN_H
#include "set_info.h"

class Set_Login : public Set_Info
{
public:
    Set_Login();
    QVariant loginUsrPwd(int type, int id);
    int loginSet(uchar type, const QVariant &v, int id);
    int loginPermit(){return mPermit;}

private:
    int loginTryLock();
    void loginLocking(bool ok);
    int loginAuth(const QStringList &ls);
    int loginCheck(const QString &str);
    int reserveCheck(const QString &str, int id);

private:
    int mFailCnt=0;
    QDateTime mDt;
    int mPermit=0;
};

#endif // SET_LOGIN_H
