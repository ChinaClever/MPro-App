/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_core.h"
#include "cascade_core.h"

Set_Core::Set_Core()
{
    Set_ReadWrite::bulid();
}

Set_Core *Set_Core::bulid()
{
    static Set_Core* sington = nullptr;
    if(sington == nullptr) {
        sington = new Set_Core();
    }
    return sington;
}

void Set_Core::writeSettings()
{
    Set_ReadWrite::bulid()->writeSettings();
}

bool Set_Core::setString(sStrItem &it)
{
    bool ret = true;
    if(it.addr) {
        ret = Cascade_Core::bulid()->masterSetString(it);
    } else {
        switch (it.fc) {
        case 10: ret = outputNameSet(it.addr, it.id, it.str); break;
        case 11: ret = setUut(it.id, it.str, it.rw); break;
        default: ret = false; qDebug() << Q_FUNC_INFO; break;
        }
    }

    return ret;
}

bool Set_Core::setting(sDataItem &it)
{
    bool ret = true;
    if(it.rw) {
        if(it.addr) {
            ret = Cascade_Core::bulid()->masterSet(it);
        } else if(it.topic == DTopic::Relay) {
            ret = relaySet(it);
        } else {
            ret = setAlarm(it);
        }
    } else ret = false;

    return ret;
}

