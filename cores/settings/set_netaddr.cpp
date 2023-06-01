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
    App_Core *app = App_Core::bulid();
    sNetInterface net = cm::dataPacket()->net;
    sNetAddr *inet = &net.inet; QVariant res;
    if(0 == sub) app->inet_readCfg(*inet, "IPV4");
    else {inet = &net.inet6; app->inet_readCfg(*inet, "IPV6");}
    if(inet->dhcp || strlen(inet->ip) < 3) net = cm::dataPacket()->net;

    switch (fc) {
    case 0: res = inet->en; break;
    case 1: res = inet->dhcp; break;
    case 2: res = inet->ip; break;
    case 3: res = inet->mask; break;
    case 4: res = inet->gw; break;
    case 5: res = inet->prefixLen; break;
    case 6: res = inet->dns; break;
    case 7: res = inet->dns2; break;
    case 10: res = net.name; break;
    case 11: res = net.mac; break;
    case 16: res = inet->reserve[0]; break;
    case 17: res = inet->reserve[1]; break;
    case 18: res = inet->reserve[2]; break;
    default: cout << sub << fc; break;
    }
    return res;
}

bool Set_NetAddr::netAddrSet(sCfgItem &it, const QVariant &v)
{    
    sNetInterface *net = &(cm::dataPacket()->net);
    sNetAddr *inet = &net->inet; bool res = true;
    char *ptr = nullptr; if(it.id) inet = &net->inet6;

    switch (it.fc) {
    case 0: inet->en = v.toInt(); //break;
    case 15: App_Core::bulid()->inet_saveCfg(it.id); break;
    case 1: inet->dhcp = v.toInt(); break;
    case 2: ptr = inet->ip; break;
    case 3: ptr = inet->mask; break;
    case 4: ptr = inet->gw;  break;
    case 5: inet->prefixLen = v.toInt(); break;
    case 6: ptr = inet->dns;  break;
    case 7: ptr = inet->dns2; break;
    case 11: ptr = net->mac; system(QStringLiteral("echo '%1' > /usr/data/clever/cfg/mac.ini").arg(v.toString()).toLocal8Bit().data()); break;
    default: res = false; cout << it.fc; break;
    } //cout << it.id << it.fc << v;

    if(ptr) {
        QByteArray array = v.toByteArray();
        qstrcpy(ptr, array.data());
        ptr[v.toByteArray().size()] = 0;
    }

    return res;
}
