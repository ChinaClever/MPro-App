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
    bool ret = false;
    if(unit.rw) {
        ret = upIndexValue(unit);
        if(ret) oplog(unit);
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


QString Set_Alarm::opContent(const sDataItem &index)
{
    QString str;
    double rate = 1;

    switch (index.topic) {
    case DTopic::Vol: rate = COM_RATE_VOL; break;
    case DTopic::Cur: rate = COM_RATE_CUR; break;
    case DTopic::Pow: rate = COM_RATE_POW; break;
    case DTopic::Tem: rate = COM_RATE_TEM; break;
    case DTopic::Hum: rate = COM_RATE_HUM; break;
    }

    switch (index.subtopic) {
    case DSub::Rated: str = QObject::tr("额定值"); break;
    case DSub::VMax: str = QObject::tr("报警最大值"); break;
    case DSub::VMin: str = QObject::tr("报警最大值"); break;
    case DSub::VCrMax: str = QObject::tr("预警最大值"); break;
    case DSub::VCrMin: str = QObject::tr("预警最大值"); break;
    case DSub::EnAlarm: str = QObject::tr("报警开关"); break;
    default: qDebug() << Q_FUNC_INFO; break;
    }

    str += QObject::tr("修改为:%1").arg(index.value/rate);
    return str;
}

void Set_Alarm::oplog(const sDataItem &it)
{
    QString content = QObject::tr("全部");
    if(it.id) content = QObject::tr("第%１ ").arg(it.id);
    content += Alarm_Log::bulid()->alarmType(it);
    content += opContent(it);

    sOpItem db;
    db.content = content;
    db.op_src = opSrc(it.txType);
    Log_Core::bulid()->append(db);
}
