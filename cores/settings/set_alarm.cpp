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
        ret = upMetaData(unit);
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
    case TxJson: str = "JSON"; break;
    case TxSsh: str = "SSH"; break;
    case TxWebocket: str = "WebSocket"; break;
    default: qDebug() << Q_FUNC_INFO; break;
    }
    return str;
}


QString Set_Alarm::opContent(const sDataItem &index)
{
    double rate = 1;
    QString str, suffix;

    switch (index.topic) {
    case DTopic::Vol: rate = COM_RATE_VOL; suffix = "V"; break;
    case DTopic::Cur: rate = COM_RATE_CUR; suffix = "A"; break;
    case DTopic::Pow: rate = COM_RATE_POW; suffix = "KW"; break;
    case DTopic::Tem: rate = COM_RATE_TEM; suffix = "°C"; break;
    case DTopic::Hum: rate = COM_RATE_HUM; suffix = "%";break;
    }

    switch (index.subtopic) {
    case DSub::Rated: str = QStringLiteral("额定值"); break;
    case DSub::VMax: str = QStringLiteral("报警最大值"); break;
    case DSub::VMin: str = QStringLiteral("报警最小值"); break;
    case DSub::VCrMax: str = QStringLiteral("预警最大值"); break;
    case DSub::VCrMin: str = QStringLiteral("预警最小值"); break;
    case DSub::EnAlarm: str = QStringLiteral("报警开关"); break;
    default: qDebug() << Q_FUNC_INFO; break;
    }

    str += QStringLiteral("修改为:%1 ％2").arg(index.value/rate).arg(suffix);
    return str;
}

void Set_Alarm::oplog(const sDataItem &it)
{
    QString content = QStringLiteral("全部");
    if(it.id) content = QStringLiteral("第%１ ").arg(it.id);
    content += Alarm_Log::bulid()->alarmType(it);
    content += opContent(it);

    sOpItem db;
    db.content = content;
    db.op_src = QStringLiteral("告警设置"); //opSrc(it.txType);
    Log_Core::bulid()->append(db);
}
