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
    if(mCfg->enV3) {
        startSnmpdV3();
    } else {
        startSnmpd();
    }
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
    } else if(QFile::exists("/etc/"+fn)) {
        cmd += "/etc/"+ fn + " &";
    } else {
        cmd.clear();
    }

    qDebug() << cmd;
    if(cmd.size()) system(cmd.toLatin1().data());
    else qDebug() << "Error: start snmpd error";
}
