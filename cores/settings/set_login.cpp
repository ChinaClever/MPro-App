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
    case 4: ptr = it->permit[id]; break;
    default:  qDebug() << Q_FUNC_INFO; break;
    }

    return ptr;
}


bool Set_Login::loginSet(uchar type, char *str, uchar txType, int id)
{
    QString key; bool ret = true;
    QString prefix = "login"; char *ptr=nullptr;
    sDevLogin *it = &(cm::dataPacket()->login);

    switch (type) {
    case 1: key = "user"; ptr = it->user[id]; break;
    case 2: key = "pwd"; ptr = it->pwd[id]; break;
    case 3: key = "token"; ptr = it->token[id]; break;
    case 4: key = "permit"; ptr = it->permit[id]; break;
    default: ret = false; qDebug() << Q_FUNC_INFO; break;
    }

    if(ptr) {
        qstrcpy(ptr, str);
        Cfg_Obj *cfg = Cfg_Obj::bulid();
        cfg->writeCfg(key, QString(ptr), prefix);

        sOpItem db; db.op_src = opSrc(txType);
        db.content = QObject::tr("%1 修改为 %2").arg(key, str);
        Log_Core::bulid()->append(db);
    }
    //Cfg_ReadWrite::bulid()->writeSettings();

    return ret;
}
