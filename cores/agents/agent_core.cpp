/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "agent_core.h"


Agent_Core::Agent_Core(QObject *parent)
    : Agent_Trap{parent}
{
    mCfg = &snmpCfg;
    //if(mCfg->enV3) {
    //   startSnmpdV3();
    //} else {
    startSnmpd();
    //}
}

Agent_Core *Agent_Core::bulid(QObject *parent)
{
    static Agent_Core* sington = nullptr;
    if(sington == nullptr) {
        sington = new Agent_Core(parent);
    }
    return sington;
}

void Agent_Core::startSnmpdV3()
{
    qDebug() << "startSnmpdV3"  << mCfg->enV3;
}

void Agent_Core::startSnmpd()
{
    QString fn = "snmpd.conf";
    QString cmd = "snmpd -f -Lo -C -c ";
    QString ret = cm::execute("proc_run snmpd");
    if(ret.toInt()) return ;

    QString custom = "/usr/data/clever/cfg/";
    if(QFile::exists(custom + fn)) {
        cmd += custom + fn + " &";
    } else if(QFile::exists("/usr/data/etc/"+fn)) {
        cmd += "/usr/data/etc/"+ fn + " &";
    } else {
        cmd.clear();
    }

    qDebug() << cmd << mCfg->enV2 << mCfg->enV3;
    if(cmd.size()) system(cmd.toLatin1().data());
    else qDebug() << "Error: start snmpd error";
}

QByteArray Agent_Core::snmdConf()
{
    QString fn = "/usr/data/clever/cfg/snmpd.conf";
    QByteArray res; QFile file(fn);
    if(file.open(QIODevice::ReadOnly)) {
        res = file.readAll();
    } file.close();
    return res;
}

void Agent_Core::set_snmpdV3()
{
#if 0
    QString fmd = "\nrwuser %1\n createUser %1 MD5 \"%2\" %4 \"%3\"";
    QString cmd = fmd.arg(mCfg->usr, mCfg->pwd, mCfg->key, mCfg->encrypt?"AES":"DES");
    QString res = snmdConf(); int idx = res.indexOf("\nrwuser");
    if(idx > 0 ) res = res.mid(0, idx);
    if(mCfg->enV3) res.append(cmd);

    QString fn = "/usr/data/clever/cfg/snmpd.conf"; QFile file(fn);
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.write(res.toLatin1());
    } file.close();
#else
    QString fn = "/usr/data/clever/cfg/snmpd.conf";
    QString str, fmd; if(mCfg->enV3) {
        str = "sed -i '57crwuser %1' " + fn;
        system(str.arg(mCfg->usr).toStdString().c_str());

        fmd = "sed -i '58ccreateUser %1 MD5 \"%2\" %4 \"%3\"' " + fn;
        str = fmd.arg(mCfg->usr, mCfg->pwd, mCfg->key, mCfg->encrypt?"AES":"DES");
        system(str.arg(mCfg->usr).toStdString().c_str());
    } else {
        QString str = "sed -i '57c ' " + fn;
        system(str.toStdString().c_str());
        str = "sed -i '58c ' " + fn;
        system(str.toStdString().c_str());
    }

    str = "sed -i '32crocommunity %1' " + fn;
    system(str.arg(mCfg->get).toStdString().c_str());

    str = "sed -i '35crocommunity6 %1' " + fn;
    system(str.arg(mCfg->get).toStdString().c_str());

    str = "sed -i '38ccom2sec readonly default  %1' " + fn;
    system(str.arg(mCfg->get).toStdString().c_str());

    str = "sed -i '33crwcommunity %1' " + fn;
    system(str.arg(mCfg->set).toStdString().c_str());

    str = "sed -i '36crwcommunity6 %1' " + fn;
    system(str.arg(mCfg->set).toStdString().c_str());

    str = "sed -i '39com2sec readwrite default  %1' " + fn;
    system(str.arg(mCfg->set).toStdString().c_str());

#endif
}

