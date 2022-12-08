/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_ssh.h"

sSshCfg App_Ssh::sshCfg;
App_Ssh::App_Ssh(QObject *parent)
    : App_Smtp{parent}
{
    QTimer::singleShot(111,this,SLOT(ssh_initFunSlot()));
}

void App_Ssh::ssh_initFunSlot()
{
    if(!sshCfg.ssh_en) system("killall sshd");
    if(!sshCfg.telnet_en) system("killall telnetd");
}

void App_Ssh::ssh_delUser()
{
    QString usr, cmd = "cut -d: -f1 /usr/data/etc/passwd";
    QStringList ls = cm::execute(cmd).split("\n");
    if(ls.size() > 13){
        usr = ls.last();
        cmd = "deluser -r " + usr;
        system(cmd.toLatin1().data());
    }
}

bool App_Ssh::ssh_addrUser()
{
    QString cmd = "usercfg %1 %2";
    QString str = cmd.arg(sshCfg.usr, sshCfg.pwd);
    return system(str.toLatin1().data());
}

bool App_Ssh::ssh_save()
{
    ssh_delUser();
    return ssh_addrUser();
}
