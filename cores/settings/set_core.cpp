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
    bool ret = false;
    if(it.rw) {
        if(it.addr) {
            ret = Cascade_Core::bulid()->masterSetString(it);
        } else {
            switch (it.fc) {
            case SFnCode::OutputName: ret = outputNameSet(it); break;
            case SFnCode::Uuts: ret = setUut(it.id, it.str, it.txType); break;
            default: qDebug() << Q_FUNC_INFO << it.fc; break;
            }
        }
    } else qDebug() << Q_FUNC_INFO;

    return ret;
}

QString Set_Core::getString(sStrItem &it)
{
    QString str;
    if(!it.rw) {
        switch (it.fc) {
        case SFnCode::OutputName: str = outputName(it.addr, it.id); break;
        case SFnCode::Uuts: str = getUut(it.addr, it.id); break;
        default: qDebug() << Q_FUNC_INFO << it.fc; break;
        }
    }

    return str;
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
    } else {
        ret = false;
        qDebug() << Q_FUNC_INFO;
    }

    return ret;
}

