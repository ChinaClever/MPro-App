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

void App_Shell::shell_readProcess()
{
    QMap<int, QProcess *>::iterator it;
    for(it=mMap.begin(); it != mMap.end(); it++) {
        int id = it.key();
        auto pro = it.value();
        if(pro->isReadable()) {
            shellCfg.result[id] << pro->readAllStandardError();
            shellCfg.result[id] << pro->readAllStandardOutput();
            if(shellCfg.result[id].size() >100) shellCfg.result[id].clear();
            //qDebug().noquote() << id << shellCfg.result[id];
        }
    }
}

void App_Shell::shell_execute(int id)
{
    if(shellCfg.cmd[id].size()) {
        QString fmd = "echo '%1' > %2";
        QString fn = QString("/tmp/script/%1.sh").arg(id);
        system(fmd.arg(shellCfg.cmd[id],fn).toStdString().c_str());
        QString cmd = "chmod +x %1"; cm::execute(cmd.arg(fn));

        QProcess *pro = new QProcess(this);
        QStringList arguments; arguments << fn;
        pro->setProgram("sh"); pro->setArguments(arguments);
        //pro->setProcessChannelMode(QProcess::MergedChannels);   //设置读取标准输出模式
        connect(pro, &QProcess::readyReadStandardError,[this](){this->shell_readProcess();});
        connect(pro, &QProcess::readyReadStandardOutput,[this](){this->shell_readProcess();});
        mMap[id] = pro; pro->start();
    }
}

void App_Shell::shell_initSlot()
{
    //shellCfg.startup[0] = 1;
    //shellCfg.cmd[0] ="ls -l /";

    for(int i=0; i<SHELL_SIZE; ++i) {
        if(shellCfg.startup[i]) shell_execute(i);
    }
}
