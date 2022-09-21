/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_netaddr.h"
#include "app_core.h"

Set_NetAddr::Set_NetAddr()
{

}

QVariant Set_NetAddr::netAddrCfg(uchar fc, uchar sub)
{    
    sNetInterface *net = &(cm::dataPacket()->net);
    sNetAddr *inet = &net->inet; QVariant res;
    if(sub) inet = &net->inet6;

    switch (fc) {
    case 0: res = inet->en; break;
    case 1: res = inet->mode; break;
    case 2: res = inet->ip; break;
    case 3: res = inet->mask; break;
    case 4: res = inet->gw; break;
    case 5: res = inet->prefixLen; break;
    case 6: res = inet->dns; break;
    case 7: res = inet->dns2; break;
    default: qDebug() << Q_FUNC_INFO; break;
    }
    return res;
}

bool Set_NetAddr::netAddrSet(sCfgItem &it, const QVariant &v)
{    
    sNetInterface *net = &(cm::dataPacket()->net);
    sNetAddr *inet = &net->inet; bool res = true;
    if(it.sub) inet = &net->inet6;

    switch (it.fc) {
    case 0: inet->en = v.toInt(); break;
    case 1: inet->mode = v.toInt(); break;
    case 2: qstrcpy(inet->ip, v.toByteArray().data()); break;
    case 3: qstrcpy(inet->mask, v.toByteArray().data()); break;
    case 4: qstrcpy(inet->gw, v.toByteArray().data());  break;
    case 5: inet->prefixLen = v.toInt(); break;
    case 6: qstrcpy(inet->dns, v.toByteArray().data());  break;
    case 7: qstrcpy(inet->dns2, v.toByteArray().data()); break;
    default: res = false; qDebug() << Q_FUNC_INFO; break;
    } if(res) App_Core::bulid()->inet_setInterface();

    return res;
}
