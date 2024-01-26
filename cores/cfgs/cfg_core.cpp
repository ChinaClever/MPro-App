/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cfg_core.h"

Cfg_Core::Cfg_Core()
{
    readSettings();
}

Cfg_Core *Cfg_Core::bulid()
{
    static Cfg_Core* sington = nullptr;
    if(sington == nullptr) {
        sington = new Cfg_Core();
    }
    return sington;
}

void Cfg_Core::writeAlarmDefault()
{
    QtConcurrent::run(this,&Cfg_Core::slaveAlarmDefault);
}

void Cfg_Core::slaveAlarmDefault()
{    
    QString cmd = "rm -f %1";
    QString dir = "/usr/data/pdu/cfg/";
    system(cmd.arg(dir+CFG_ALARM_DF).toLatin1().data());
    cmd = "cp -af %1 %2"; writeAlarms(); cm::mdelay(1543);
    cmd = cmd.arg(dir+CFG_ALARM_FN, dir+CFG_ALARM_DF);
    system(cmd.toLatin1().data()); qDebug() << cmd;
}

bool Cfg_Core::readSettings()
{
    bool ret = readAlarm(CFG_ALARM_FN); /*读取"alarm.conf"文件*/
    if(!ret) ret = readAlarm(CFG_ALARM_DF); /*如果前一次读取报警设置失败则读取"alarm.cfg"文件*/
    if(!ret) ret = initialData();  /*如果前二次读取报警设置失败则初始化数据*/

    return ret;
}

