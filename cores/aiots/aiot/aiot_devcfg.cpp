/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "aiot_devcfg.h"

Aiot_DevCfg::Aiot_DevCfg()
{

}

void Aiot_DevCfg::basePost()
{
    QString module;

    int v = mDevData->status;
    QString key = "runningState";
    appendProperty(key, v, module);

    key = "location";
    QString str = mDevData->cfg.uut.location;
    appendProperty(key, str, module);
}

void Aiot_DevCfg::devCfgPost()
{
    QString key = "lineNum";
    QString module = "DevCfg";

    int v = mDevData->cfg.nums.lineNum;
    appendProperty(key, v, module);

    key = "loopNum";
    v = mDevData->cfg.nums.loopNum;
    appendProperty(key, v, module);

    key = "outletNum";
    v = mDevData->cfg.nums.outputNum;
    appendProperty(key, v, module);
}
