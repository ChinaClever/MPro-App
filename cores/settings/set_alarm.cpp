/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_alarm.h"
#include "odbc_core.h"

Set_Alarm::Set_Alarm()
{

}

bool Set_Alarm::setAlarm(sDataItem &unit)
{   
    return upMetaData(unit);
}

void Set_Alarm::setAlarmLog(sDataItem &unit)
{
    oplog(unit);
    Odbc_Core::bulid()->threshold(unit);
}

QString Set_Alarm::opSrc(uchar addr)
{
    QString str;
    if(addr == 0xff) {
        str = QStringLiteral("所有级联设备");
    } if(addr) {
        str = QStringLiteral("副机%1").arg(addr);
    } else str = QStringLiteral("本机");

    return str;
}


QString Set_Alarm::opContent(const sDataItem &index)
{
    double rate = 1;
    QString str, suffix;

    switch (index.topic) {
    case DTopic::Vol: rate = COM_RATE_VOL; suffix = "V"; break;
    case DTopic::Cur: rate = COM_RATE_CUR; suffix = "A"; break;
    case DTopic::Pow: rate = COM_RATE_POW; suffix = "kW"; break;
    case DTopic::Tem: rate = COM_RATE_TEM; suffix = "°C"; break;
    case DTopic::Hum: rate = COM_RATE_HUM; suffix = "%";break;
    }

    switch (index.subtopic) {
    case DSub::Rated: str = QStringLiteral("额定值"); break;
    case DSub::VMax: str = QStringLiteral("报警最大值"); break;
    case DSub::VMin: str = QStringLiteral("报警最小值"); break;
    case DSub::VCrMax: str = QStringLiteral("预警最大值"); break;
    case DSub::VCrMin: str = QStringLiteral("预警最小值"); break;
    case DSub::EnAlarm: str = QStringLiteral("报警开关"); rate = 1; break;
    default: qDebug() << Q_FUNC_INFO; break;
    }

    str += QStringLiteral("修改为:%1 %2").arg(index.value/rate).arg(suffix);
    return str;
}

void Set_Alarm::oplog(const sDataItem &it)
{
    QString content = QStringLiteral("全部");
    if(it.id) content = QStringLiteral("第%１ ").arg(it.id);
    content += Alarm_Log::bulid()->alarmType(it);
    content += opContent(it);

    sEventItem db;
    db.addr = it.addr;
    db.event_content = content;
    db.event_type = QStringLiteral("告警设置:");
    db.event_type += opSrc(it.txType);
    Log_Core::bulid()->append(db);
}
