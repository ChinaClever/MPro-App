/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_login.h"
#include "app_core.h"

Set_Login::Set_Login()
{
    mDt = QDateTime::currentDateTime();
}


QVariant Set_Login::loginUsrPwd(int type, int id)
{
    sDevLogin *it = &(cm::dataPacket()->login[id]);
    QVariant res; switch (type) {
    case 1: res = it->user; break;
    case 2: res = it->pwd; break;
    case 3: res = it->token; break;
    case 4: res = it->permit; break;
    case 5: res = it->groupCtrl; break;
    default:  qDebug() << Q_FUNC_INFO; break;
    }

    return res;
}


int Set_Login::loginSet(uchar type, const QVariant &v, int id)
{
    QString key; int ret = true;
    QString prefix = "login"; char *ptr=nullptr;
    sDevLogin *it = &(cm::dataPacket()->login[id]);

    switch (type) {
    case 1: key = "user_%1"; ptr = it->user; break;
    case 2: key = "pwd_%1"; ptr = it->pwd; break;
    case 3: key = "token_%1"; ptr = it->token; break;
    case 4: key = "permit_%1"; it->permit = v.toInt(); break;
    case 5: key = "groupCtrl_%1"; it->groupCtrl = v.toInt(); break;
    case 11: ret = loginCheck(v.toString()); break;
    default: ret = false; qDebug() << Q_FUNC_INFO; break;
    } //if(ret && (type != 11)) Cfg_ReadWrite::bulid()->writeParams();

    if(ret && key.size()) {
        Cfg_Com *cfg = Cfg_Com::bulid();
        cfg->writeCfg(key.arg(id), v, prefix);
    }

    if(ptr) {
        QByteArray str = v.toByteArray();
        qstrcpy(ptr, str.data()); //ptr[v.toByteArray().size()] = 0;
        //        sEventItem db; db.event_type = QStringLiteral("登陆信息"); //opSrc(txType);
        //        db.event_content = QStringLiteral("%1 修改为 %2").arg(key, v.toString());
        //        Log_Core::bulid()->append(db);
    }

    return ret;
}

bool Set_Login::loginAuth(const QStringList &ls)
{
    bool ret = false;
    for(int i=0; i<USER_NUM; ++i) {
        sDevLogin *it = &(cm::dataPacket()->login[i]);
        QString usr = it->user, pwd = it->pwd;
        if((ls.first() == usr) && (ls.last() == pwd)) {
            ret = true; break;
        }
    }
    return ret;
}

/**
 * 1、输入五次账号密码锁定十分钟。
 * 2、连续输入错15次账号密码锁定60分钟。
 * 3、连续输入错16次或者密码锁定61分钟。
 * 4、连续输入错17次或者密码锁定62分钟。
 * 5、连续输入错18次或者密码锁定63分钟。
 */
int Set_Login::loginTryLock()
{
    int ret = 0; if(mFailCnt > 4) {
        QDateTime dt = QDateTime::currentDateTime();
        int minutes = mDt.secsTo(dt) / 60;
        ret = (mFailCnt-4) * 10 - minutes;
        if(ret < 0) ret = 0;
    }

    return ret;
}

void Set_Login::loginLocking(bool ok)
{
    mDt = QDateTime::currentDateTime();
    if(ok) mFailCnt=0; else mFailCnt += 1;
}

int Set_Login::loginCheck(const QString &str)
{
    QStringList ls = str.split(";");
    int ret = loginTryLock();
    if(ret) return 100+ret;

    if(ls.size() == 2) {
        sRadiusCfg *cfg = &App_Radius::radiusCfg;
        if(cfg->en) {
            int res = App_Core::bulid()->radius_work(ls.first(), ls.last());
            if((res == -1) && cfg->local) ret = loginAuth(ls);
            else ret = res;
        } else {
            ret = loginAuth(ls);
        }
    } else if(App_Ldap::ldapCfg.en && ls.size()) {
        bool res = App_Core::bulid()->ldap_work(ls.last());
        if(res) ls.insert(0, "ldap:"+App_Ldap::ldapCfg.user);
    }

    loginLocking(ret); if(ret) {
        sEventItem db; db.event_type = QStringLiteral("用户登陆");
        db.event_content = QStringLiteral("登陆账号为 %1").arg(ls.first());
        Log_Core::bulid()->append(db); ret = 1;
    } else ret = 0; //cout << ls << ret;

    return ret;
}
