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
    sDevLogin *it = &(cm::dataPacket()->login[id]);
    char *ptr = nullptr; switch (type) {
    case 1: ptr = it->user; break;
    case 2: ptr = it->pwd; break;
    case 3: ptr = it->token; break;
    case 4: ptr = it->permit; break;
    default:  qDebug() << Q_FUNC_INFO; break;
    }

    return ptr;
}


bool Set_Login::loginSet(uchar type, char *str, int id)
{
    QString key; bool ret = true;
    QString prefix = "login"; char *ptr=nullptr;
    sDevLogin *it = &(cm::dataPacket()->login[id]);

    switch (type) {
    case 1: key = "user"; ptr = it->user; break;
    case 2: key = "pwd"; ptr = it->pwd; break;
    case 3: key = "token"; ptr = it->token; break;
    case 4: key = "permit"; ptr = it->permit; break;
    case 11: ret = loginCheck(str); break;
    default: ret = false; qDebug() << Q_FUNC_INFO; break;
    } //if(ret && (type != 11)) Cfg_ReadWrite::bulid()->writeParams();

    if(ptr) {
        qstrcpy(ptr, str);
        Cfg_Obj *cfg = Cfg_Obj::bulid();
        cfg->writeCfg(key, QString(ptr), prefix);

        sOpItem db; db.op_src = QObject::tr("登陆信息"); //opSrc(txType);
        db.content = QObject::tr("%1 修改为 %2").arg(key, str);
        Log_Core::bulid()->append(db);
    }

    return ret;
}


bool Set_Login::loginCheck(const QString &str)
{
    QStringList ls = str.split("; ");
    bool ret = false; if(ls.size() == 2) {
        sDevLogin *it = &(cm::dataPacket()->login[0]);
        QString usr = it->user, pwd = it->pwd;
        if((ls.first() == usr) && (ls.last() == pwd)) {
            ret = true;
            sOpItem db; db.op_src = QObject::tr("用户登陆"); //opSrc(txType);
            db.content = QObject::tr("登陆登陆为 %1").arg(str);
            Log_Core::bulid()->append(db);
        }
    }
    return ret;
}
