/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_login.h"

Set_Login::Set_Login()
{

}

QString Set_Login::loginUsrPwd(int type, int id)
{
    sDevLogin *it = &(cm::dataPacket()->login);
    char *ptr = nullptr; switch (type) {
    case 1: ptr = it->user[id]; break;
    case 2: ptr = it->pwd[id]; break;
    case 3: ptr = it->token[id]; break;
    default:  qDebug() << Q_FUNC_INFO; break;
    }

    return ptr;
}


bool Set_Login::loginSet(uchar type, char *str, uchar txType, int id)
{
    bool ret = true; char *ptr=nullptr;
    sDevLogin *it = &(cm::dataPacket()->login);

    switch (type) {
    case 1: ptr = it->user[id]; break;
    case 2: ptr = it->pwd[id]; break;
    case 3: ptr = it->token[id]; break;
    default: ret = false; qDebug() << Q_FUNC_INFO; break;
    }

    if(ptr) qstrcpy(ptr, str);
    Cfg_ReadWrite::bulid()->writeSettings();

    return ret;
}
