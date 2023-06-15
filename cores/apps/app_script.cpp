/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_script.h"
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

sScriptCfg App_Script::scriptCfg;
App_Script::App_Script(QObject *parent)
    : App_NetAddr{parent}
{
    mScriptTimer = new QTimer(this); mScriptTimer->start(1500);
    QTimer::singleShot(1234,this,&App_Script::script_initSlot);
    connect(mScriptTimer, &QTimer::timeout, this, &App_Script::script_readProcess);
}

void App_Script::script_readProcess()
{
    static char buffer[2048] = {0};
    QMap<int, FILE *>::iterator it;
    for(it=mMap.begin(); it != mMap.end(); it++) {
        int id = it.key(); auto pro = it.value();
#if 0
        if(pro->isReadable()) {
            QString res = pro->readAllStandardError().trimmed() ;
            scriptCfg.result[id].insert(0, res);
            res = pro->readAllStandardOutput().trimmed(); //simplified()
            scriptCfg.result[id].insert(0, res);

        }
#else
        buffer[0] = 0; while(fgets(buffer,sizeof(buffer), pro)) {
            if(strlen(buffer)) scriptCfg.result[id].insert(0, buffer);
            //qDebug().noquote() << id << scriptCfg.result[id];
        } if(scriptCfg.result[id].size()>100) scriptCfg.result[id].removeLast();
#endif
    }
}

void App_Script::script_kill(int id)
{
    if(mMap.contains(id)) {
        QString cmd = "killall " + mCmdMap[id];
        system(cmd.toStdString().c_str());
        pclose(mMap[id]); //mMap[id]->kill();
    } mMap.remove(id);
}

void App_Script::script_execute(int id)
{
    if(scriptCfg.cmd[id].size()) {
        QString fmd = "echo '%1' > %2";
        QString fn = QString("/tmp/script/s_%1").arg(id);
        system(fmd.arg(scriptCfg.cmd[id],fn).toStdString().c_str());
        QString cmd = "chmod +x %1"; cm::execute(cmd.arg(fn));

        QString program = "sh";
        switch (scriptCfg.type[id]) {
        case 1: program = "micropython"; break;
        case 2: program = "lua"; break;
        }
#if 0
        QProcess *pro = new QProcess(this);
        QStringList arguments; arguments << fn;
        pro->setProgram(program); pro->setArguments(arguments);
        //pro->setProcessChannelMode(QProcess::MergedChannels);   //设置读取标准输出模式
        connect(pro, &QProcess::readyReadStandardError,[this](){this->script_readProcess();});
        connect(pro, &QProcess::readyReadStandardOutput,[this](){this->script_readProcess();});
        script_kill(id); mMap[id] = pro; pro->start();
#else
        mCmdMap[id] = cmd = program + " " + fn;
        char *ptr = cmd.toLatin1().data();
        script_kill(id); FILE *fp = popen(ptr, "r");
        if(fp) mMap[id] = fp; else cout << id << program;
        int fd = fileno(fp); int flags = fcntl(fd, F_GETFL, 0);
        fcntl(fd, F_SETFL, flags | O_NONBLOCK); // 设置文件描述符为非阻塞模式
        //cout << id << cmd;
#endif
    }
}

void App_Script::script_initSlot()
{
    //scriptCfg.type[0] = 1;
    //scriptCfg.startup[0] = 1;
    //scriptCfg.cmd[0] ="ls -l /";
    //scriptCfg.cmd[0] ="print(\"Hello, World!\")";

    for(int i=0; i<SCRIPT_SIZE; ++i) {
        if(scriptCfg.startup[i]) script_execute(i);
    }
}
