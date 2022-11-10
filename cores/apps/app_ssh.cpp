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
    QTimer::singleShot(1,this,SLOT(ssh_initFunSlot()));
}

void App_Ssh::ssh_initFunSlot()
{

}
