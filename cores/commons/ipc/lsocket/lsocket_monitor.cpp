/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "lsocket_monitor.h"

LSocket_Monitor::LSocket_Monitor(QObject *parent) : SM_Obj{parent}
{

}

LSocket_Monitor *LSocket_Monitor::bulid(QObject *parent)
{
    static LSocket_Monitor *sington = nullptr;
    if(!sington) sington = new LSocket_Monitor(parent);
    return sington;
}

QStringList LSocket_Monitor::onLines()
{
    QStringList res;
    foreach(auto &i, mOnLines) res << i;
    return res;
}

QStringList LSocket_Monitor::offLines()
{
    QStringList res;
    foreach(auto &i, mOffLines) res << i;
    return res;
}
