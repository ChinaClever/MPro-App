/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_alarm.h"
#include "cascade_core.h"

Set_Alarm::Set_Alarm()
{
}


bool Set_Alarm::setAlarm(sDataItem &unit)
{
    bool ret = true;
    if(unit.rw) {
        ret = upIndexValue(unit);
        if(ret && unit.rw) {

            Set_ReadWrite::bulid()->writeSettings();
        }
    }
    return ret;
}

QString Set_Alarm::opSrc(uchar txType)
{
    QString str;
    switch (txType) {
    case TxWeb: str = "WEB"; break;
    case TxModbus: str = "Modbus"; break;
    case TxSnmp: str = "SNMP"; break;
    case TxRpc: str = "RPC"; break;
    case TxUdp: str = "UDP"; break;
    case TxTcp: str = "TCP"; break;
    case TxSsh: str = "SSH"; break;
    case TxWebocket: str = "WebSocket"; break;
    default: qDebug() << Q_FUNC_INFO; break;
    }
    return str;
}
