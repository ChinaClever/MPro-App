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

bool Set_Alarm::thresholdSlave(int fc)
{
    Cfg_Core *cfg = Cfg_Core::bulid();
    bool ret = true; switch (fc) {
    case 1: cfg->writeAlarms(); break;
    case 2: cfg->writeAlarmDefault(); break;
    default: cout << fc; ret = false; break;
    }

    return ret;
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

QString Set_Alarm::opSrc(uchar addr, uchar txType)
{
    QString str;
    switch (txType) {
    case DTxType::TxWeb: str = QStringLiteral("[Web] "); break;
    case DTxType::TxSnmp: str = QStringLiteral("[SNMP] "); break;
    case DTxType::TxModbus: str = QStringLiteral("[Modbus] "); break;
    case DTxType::TxSsh: str = QStringLiteral("[SSH/Telnet] "); break;
    case DTxType::TxRest: str = QStringLiteral("[REST] "); break;
    case DTxType::TxRpc: str = QStringLiteral("[RPC] "); break;
    case DTxType::TxJson: str = QStringLiteral("[JSON] "); break;
    default: if(cm::cn()) str = QStringLiteral("[协议控制] "); else str = " [protocol control] "; break;
    }

    if(addr == 0xff) {
        if(cm::cn()) str +=QStringLiteral("所有级联设备") ; else str += "all cascaded devices ";
    } if(addr) {
         if(cm::cn()) str += QStringLiteral("副机"); else str +="slave ";
         str += QString::number(addr);
    } else if(cm::cn()) str += QStringLiteral("本机"); else str += "native ";

    return str+" ";
}


QString Set_Alarm::opContent(const sDataItem &index)
{
    double rate = 1;
    QString str, suffix;
    int offset = 0;

    switch (index.topic) {
    case DTopic::Vol: rate = COM_RATE_VOL; suffix = "V"; break;
    case DTopic::Cur: rate = COM_RATE_CUR; suffix = "A"; break;
    case DTopic::Pow: rate = COM_RATE_POW; suffix = "kW"; break;
    case DTopic::Tem: rate = COM_RATE_TEM; suffix = "°C"; break;
    case DTopic::Hum: rate = COM_RATE_HUM; suffix = "%";break;
    } if(DTopic::Tem == index.topic) offset = 400;

    switch (index.subtopic) {
    case DSub::Rated: if(cm::cn()) str = QStringLiteral("额定值"); else str = "rated value "; break;
    case DSub::VMax: if(cm::cn()) str = QStringLiteral("报警最大值"); else str = "alarm maximum value "; break;
    case DSub::VMin: if(cm::cn()) str = QStringLiteral("报警最小值"); else str = "alarm minimum value "; break;
    case DSub::VCrMax: if(cm::cn()) str = QStringLiteral("预警最大值"); else str = "maximum warning value "; break;
    case DSub::VCrMin: if(cm::cn()) str = QStringLiteral("预警最小值"); else str = "minimum  warning value "; break;
    case DSub::EnAlarm: if(cm::cn()) str = QStringLiteral("报警开关"); else str = "alarm switch "; rate = 1; suffix="";break;
    case DSub::DHda: if(cm::cn()) str = QStringLiteral("历史记录"); else str = "history "; rate = 1; suffix="";break;
    default: cout << index.subtopic; break;
    }

    if(cm::cn()) str += QStringLiteral("修改为:%1 %2");
    else str += QStringLiteral("modify to:%1 %2 ");
    return str.arg(((int)index.value-offset)/rate).arg(suffix);
}

void Set_Alarm::oplog(const sDataItem &it)
{
    QString content = QStringLiteral("全部");
    if(!cm::cn()){content ="all ";}if(it.id) {
        if(cm::cn()) content = QStringLiteral("第%1个").arg(it.id);
        else content = QString::number(it.id);
    } content += Alarm_Log::bulid()->alarmType(it);
    content += opContent(it);

    sEventItem db;
    db.addr = it.addr;
    db.event_content = content;
    db.event_type = opSrc(it.addr, it.txType);
    if(DTopic::Ele == it.topic) {
        if(cm::cn()) db.event_type += QStringLiteral("电能清除;");
        else db.event_type += "Electric energy clearing;";
    } else {
        if(cm::cn()) db.event_type += QStringLiteral("告警设置;");
        else db.event_type += "alarm settings;";
    } Log_Core::bulid()->append(db);
}
