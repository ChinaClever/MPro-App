/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ipc_alarmclient.h"

IPC_AlarmClient::IPC_AlarmClient(QObject *parent)
    : IPC_ObjClient{parent}
{

}

IPC_AlarmClient *IPC_AlarmClient::bulid(QObject *parent)
{
    static IPC_AlarmClient *sington = nullptr;
    if(!sington) {
        sington = new IPC_AlarmClient(parent);
        sington->initFunction(IPC_KEY_ALARM);
    }
    return sington;
}

bool IPC_AlarmClient::setAlarm(const sSetAlarmUnit &unit)
{
    QVariantList lv{cm::toByteArray(unit)};
    bool ret = inputCheck(lv);
    if(ret) ret = mDbus->sendBus(lv);
    return ret;
}
