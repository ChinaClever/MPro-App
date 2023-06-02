/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_script.h"

sScriptCfg App_Script::scriptCfg;
App_Script::App_Script(QObject *parent)
    : App_NetAddr{parent}
{
    QTimer::singleShot(1234,this,&App_Script::script_initSlot);
}

void App_Script::script_readProcess()
{
    QMap<int, QProcess *>::iterator it;
    for(it=mMap.begin(); it != mMap.end(); it++) {
        int id = it.key();
        auto pro = it.value();
        if(pro->isReadable()) {
            QString res = pro->readAllStandardError().simplified();
            if(res.size() > 1) scriptCfg.result[id].insert(0, res);
            res = pro->readAllStandardOutput().simplified();
            if(res.size() > 1) scriptCfg.result[id].insert(0, res);
            if(scriptCfg.result[id].size()>100) scriptCfg.result[id].removeLast();
            qDebug().noquote() << id << scriptCfg.result[id];
        }
    }
}

void App_Script::script_kill(int id)
{
    if(mMap.contains(id)) mMap[id]->kill();
    mMap.remove(id);
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

        QProcess *pro = new QProcess(this);
        QStringList arguments; arguments << fn;
        pro->setProgram(program); pro->setArguments(arguments);
        //pro->setProcessChannelMode(QProcess::MergedChannels);   //设置读取标准输出模式
        connect(pro, &QProcess::readyReadStandardError,[this](){this->script_readProcess();});
        connect(pro, &QProcess::readyReadStandardOutput,[this](){this->script_readProcess();});
        script_kill(id); mMap[id] = pro; pro->start();
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
