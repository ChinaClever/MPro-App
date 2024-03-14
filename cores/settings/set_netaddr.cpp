/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_netaddr.h"
#include "app_core.h"

Set_NetAddr::Set_NetAddr()
{
    m_net.inet.en = 0;
    m_net.mac[0] = 0;
}

QVariant Set_NetAddr::netAddrCfg(uchar fc, uchar sub)
{
    sNetInterface *net = &cm::dataPacket()->net;
    if(!m_net.inet.en || !strlen(m_net.mac)) m_net = *net;
    if(m_net.inet6.dhcp && net->inet6.dhcp) m_net = *net;
    if(m_net.inet.dhcp && net->inet.dhcp) m_net = *net;
    sNetAddr *inet = &m_net.inet; QVariant res;
    if(sub) inet = &m_net.inet6;

    switch (fc) {
    case 0: res = inet->en; break;
    case 1: res = inet->dhcp; break;
    case 2: res = inet->ip; break;
    case 3: res = inet->mask; break;
    case 4: res = inet->gw; break;
    case 5: res = inet->prefixLen; break;
    case 6: res = inet->dns; break;
    case 7: res = inet->dns2; break;
    case 10: res = m_net.name; break;
    case 11: res = m_net.mac; break;
    case 16: res = inet->reserve[0]; break;
    case 17: res = inet->reserve[1]; break;
    case 18: res = inet->reserve[2]; break;
    default: cout << sub << fc; break;
    } if(10 == fc) {
        QString str = cm::executeCommand("ethtool eth0 | grep Speed");
        res = res.toString() + " - " + str;
    }

    return res;
}

bool Set_NetAddr::netAddrSet(sCfgItem &it, const QVariant &v)
{
    if(11==it.fc) if(cm::cipp(v)) return false;
    if(!m_net.inet.en || !strlen(m_net.mac)) m_net = cm::dataPacket()->net;
    sNetAddr *inet = &m_net.inet; bool res = true;
    char *ptr = nullptr; if(it.id) inet = &m_net.inet6;

    switch (it.fc) {
    case 0: inet->en = v.toInt(); break;
    case 1: inet->dhcp = v.toInt(); //break;
    case 15: App_Core::bulid()->inet_saveCfg(it.id, m_net); break;
    case 2: ptr = inet->ip; break;
    case 3: ptr = inet->mask; break;
    case 4: ptr = inet->gw;  break;
    case 5: inet->prefixLen = v.toInt(); break;
    case 6: ptr = inet->dns;  break;
    case 7: ptr = inet->dns2; break;
    case 11: ptr = m_net.mac; system(QStringLiteral("echo '%1' > /usr/data/pdu/cfg/mac.conf").arg(v.toString()).toLocal8Bit().data()); break;
    default: res = false; cout << it.fc; break;
    } //cout << it.id << it.fc << v;

    if(ptr) {
        QByteArray array = v.toByteArray();
        qstrcpy(ptr, array.data());
        ptr[v.toByteArray().size()] = 0;
    }

    return res;
}
