/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_diagnose.h"

Set_Diagnose::Set_Diagnose()
{

}


QVariant Set_Diagnose::net_diagnoseCfg(int fc)
{
    sNetDgsIt *it = &mNetDgs;
    QVariant res; switch (fc) {
    case 1: res = it->pingHost;  break;
    case 2: res = it->pingCnt;  break;
    case 3: res = cm::execute(QString("ping -c%1 %2").arg(it->pingCnt).arg(it->pingHost));  break;

    case 6: res = it->routeHost;  break;
    case 7: res = it->timeout; break;
    case 8: res = cm::execute(QString("traceroute -w %1 %2").arg(it->timeout).arg(it->routeHost));  break;
    default: cout << it; break;
    }

    return res;
}

bool Set_Diagnose::net_diagnoseSet(int fc, const QVariant &v)
{
    sNetDgsIt *it = &mNetDgs;
    bool ret = true; switch (fc) {
    case 1: it->pingHost = v.toString(); break;
    case 2: it->pingCnt = v.toInt(); break;
    case 6: it->routeHost = v.toString();  break;
    case 7: it->timeout = v.toInt(); break;
    default: ret = false; cout << fc; break;
    }
    return ret;
}
