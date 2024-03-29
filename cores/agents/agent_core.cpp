/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "agent_core.h"


Agent_Core::Agent_Core(QObject *parent)
    : Agent_Trap{parent}
{
    system("rm -f /usr/data/etc/snmp/snmpd.conf");
    mCfg = &snmpCfg; startSnmpd(); //set_snmpdV3(); /*启动snmpd进程*/
    QTimer::singleShot(2333,this,&Agent_Core::startSnmpdV3);    /*启动SNMP V3功能*/
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
    QString id = "oldEngineID ";
    if(!mCfg->enV3) return; //cout << mCfg->enV3;
    system("chmod 777 /usr/data/etc/snmp/snmpd.conf");
    QString dst = "/usr/data/pdu/cfg/snmpd.conf";
    QString src = "/usr/data/etc/snmp/snmpd.conf";
    for(int i=0; i<3; ++i) {
        QString cmd = "sed -n '%1p' " + src; char buf[256]={0};
        FILE *fp = popen(cmd.arg(32+i).toStdString().c_str(),"r");
        fread(buf,1,256,fp); pclose(fp); QString res(buf);
        cmd = "sed -i '%1c%2' " + dst; //cout << res;
        //cout << cm::execute(cmd.arg(60+i).arg(res).remove("\n"));
        cmd = cmd.arg(60+i).arg(res.remove("\n")); //cout << cmd;
        system(cmd.toStdString().c_str());
        if(res.contains(id)) mCfg->oldEngineID = res.remove(id);
    } if(mCfg->enV3) qDebug() << "SNMP V3 " << id << mCfg->oldEngineID;
}

void Agent_Core::startSnmpd()
{
    QString fn = "snmpd.conf";
    QString cmd = "snmpd -f -Lo -C -c ";
    QString ret = cm::execute("proc_run snmpd");
    if(ret.toInt()) return ;    /*检查snmpd进程是否已经在运行，如果是，则函数返回*/

    QString custom = "/usr/data/pdu/cfg/";
    if(QFile::exists(custom + fn)) {
        cmd += custom + fn + " &";      /*如果文件"custom + fn"存在，将cmd追加为custom + fn + &*/
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
    QString fn = "/usr/data/pdu/cfg/snmpd.conf";
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

    QString fn = "/usr/data/pdu/cfg/snmpd.conf"; QFile file(fn);
    if(file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        file.write(res.toLatin1());
    } file.close();
#else
    QString fn = "/usr/data/pdu/cfg/snmpd.conf";
    for(int i=57; i<65; ++i) {
        QString str = "sed -i '%1c ' " + fn;
        system(str.arg(i).toStdString().c_str());
    }

    QString str, fmd; if(mCfg->enV3) {
        str = "sed -i '57crwuser %1' " + fn;
        system(str.arg(mCfg->usr).toStdString().c_str());

        fmd = "sed -i '58ccreateUser %1 MD5 \"%2\" %4 \"%3\"' " + fn;
        str = fmd.arg(mCfg->usr, mCfg->pwd, mCfg->key, mCfg->encrypt?"AES":"DES");
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

    str = "sed -i '39ccom2sec readwrite default  %1' " + fn;
    system(str.arg(mCfg->set).toStdString().c_str());

#endif
}

