/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_shell.h"

sShellCfg App_Shell::shellCfg;
App_Shell::App_Shell(QObject *parent)
    : App_NetAddr{parent}
{
    QTimer::singleShot(1234,this,&App_Shell::shell_initSlot);
}

void App_Shell::shell_execute(int id)
{
    if(shellCfg.cmd[id].size()) {
        QProcess *pro = new QProcess(this);
        pro->setProgram("/bin/bash");
        QStringList  arguments;
        arguments << shellCfg.cmd[id];
        pro->setArguments(arguments);
        pro->startDetached();
    }
}

void App_Shell::shell_initSlot()
{
    for(int i=0; i<SHELL_SIZE; ++i) {
        if(shellCfg.startup[i]) shell_execute(i);
    }
}
