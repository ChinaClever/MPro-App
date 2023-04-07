/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "alarm_log.h"

Alarm_Log::Alarm_Log(QObject *parent)
    : QObject{parent}
{
    Alarm_Updater *alarm = Alarm_Updater::bulid(this);
    connect(alarm, &Alarm_Updater::alarmSig, this, &Alarm_Log::alarmSlot);
}

Alarm_Log *Alarm_Log::bulid(QObject *parent)
{
    static Alarm_Log* sington = nullptr;
    if(sington == nullptr) {
        sington = new Alarm_Log(parent);
    }
    return sington;
}

QString Alarm_Log::getCurrentAlarm(int addr)
{
    QString res; if(addr) {
        res = m_currentAlarm[addr-1];
    } else {
        for(int i=0; i<DEV_NUM; ++i) {
            if(m_currentAlarm[i].size())
                res += m_currentAlarm[i];
        }
    }
    return res;
}

void Alarm_Log::appendAlarm(const sDataItem &index, uchar value)
{
    sAlarmItem it = alarmItem(index, value);
    //QString str = QString::number(++m_id) +"、";
    QString str = it.alarm_status + it.alarm_content;
    if(str.size()) m_currentAlarm[it.addr] += str + "\n";
    //cout << m_currentAlarm[it.addr].size() << str;
}

void Alarm_Log::appendSlaveOffline(int addr)
{
    QString str = tr("副机%1离线").arg(addr);
    m_currentAlarm[0] += str + "\n";
}

void Alarm_Log::resetAwtk()
{
    system("killall awtk");
    system("awtk &");
    cout << "awtk";
}

void Alarm_Log::generateQRcode()
{
    static QString alarm; QString str = m_currentAlarm[0];
    if(str.size()) str = str.split("\n").first();
    if(str.isEmpty()) str = cm::masterDev()->cfg.uut.qrcode;
    if((str != alarm)) { alarm = str; cm::qrcodeGenerator(str); resetAwtk();}
}

QString Alarm_Log::alarmType(const sDataItem &index)
{
    QString str;

    switch (index.type) {
    case DType::Tg: str += tr("总"); break;
    case DType::Line: str += tr("相"); break;
    case DType::Loop: str += tr("回路"); break;
    case DType::Group: str += tr("组"); break;
    case DType::Dual: str += tr("双电源"); break;
    case DType::Output: str += tr("输出位"); break;
    case DType::Env: str += tr("环境"); break;
    case DType::Sensor: str += tr("传感器"); break;
    case DType::CabTg: str += tr("机柜总"); break;
    case DType::CabLine: str += tr("机柜相"); break;
    case DType::CabLoop: str += tr("机柜回路"); break;
    }

    switch (index.topic) {
    case DTopic::Vol: str += tr("电压"); break;
    case DTopic::Cur: str += tr("电流"); break;
    case DTopic::Pow: str += tr("功率"); break;
    case DTopic::Ele: str += tr("电能"); break;
    case DTopic::Tem: str += tr("温度"); break;
    case DTopic::Hum: str += tr("湿度"); break;
    case DTopic::Door1: str += tr("门禁1"); break;
    case DTopic::Door2: str += tr("门禁２"); break;
    case DTopic::Water: str += tr("水浸"); break;
    case DTopic::Smoke: str += tr("烟雾"); break;
    case DTopic::Relay: if(index.type == DType::Loop) str += tr("断路器"); else str += tr("开关"); break;
    }

    return str;
}

QString Alarm_Log::alarmStatus(uchar value)
{
    QString state; switch (value) {
    case AlarmCode::Ok: state = tr("恢复正常"); break;
    case AlarmCode::Min: state = tr("过低告警");  break;
    case AlarmCode::CrMin: state = tr("过低预警");  break;
    case AlarmCode::CrMax: state = tr("过高预警"); break;
    case AlarmCode::Max: state = tr("过高告警");  break;
    }
    return state+"; ";
}

QString Alarm_Log::alarmContent(const sDataItem &index)
{
    double rate = 1;
    int id = index.id;
    QString str, suffix;

    switch (index.topic) {
    case DTopic::Vol: rate = COM_RATE_VOL; suffix = "V"; break;
    case DTopic::Cur: rate = COM_RATE_CUR; suffix = "A"; break;
    case DTopic::Pow: rate = COM_RATE_POW; suffix = "KW"; break;
    case DTopic::Tem: rate = COM_RATE_TEM; suffix = "°C"; break;
    case DTopic::Hum: rate = COM_RATE_HUM; suffix = "%";break;
    }

    Alarm_Object obj;
    if(index.type) {
        sAlarmUnit *unit = obj.getAlarmUnit(index);
        if(unit) {
            str  = tr("当前值:%1%6　告警最小值:%2%6 预警最小值:%3%6 预警最大值:%4%6 告警最大值:%5%6")
                    .arg(unit->value[id]/rate)
                    .arg(unit->min[id] / rate)
                    .arg(unit->crMin[id] / rate)
                    .arg(unit->crMax[id] / rate)
                    .arg(unit->max[id] / rate)
                    .arg(suffix);
        } else qDebug() << Q_FUNC_INFO;
    } else {
        sTgUnit *unit = obj.getTgAlarmUnit(index);
        if(unit) {
            str  = tr("当前值:%1%6　告警最小值:%2%6 预警最小值:%3%6 预警最大值:%4%6 告警最大值:%5%6")
                    .arg(unit->value/rate)
                    .arg(unit->min / rate)
                    .arg(unit->crMin / rate)
                    .arg(unit->crMax / rate)
                    .arg(unit->max / rate)
                    .arg(suffix);
        } else qDebug() << Q_FUNC_INFO;
    }
    return str+"; ";
}


QString Alarm_Log::alarmRelay(uchar value)
{
    QString str;
    switch (value) {
    case sRelay::OffALarm: str = tr("断开"); break;
    case sRelay::NoAlarm: str = tr("恢复"); break;
    case sRelay::LifeAlarm: str = tr("寿命"); break;
    }
    return str+"; ";
}

QString Alarm_Log::alarmSensor(uchar value)
{
    QString str = tr("恢复正常");
    if(value) str = tr("告警");
    return str+"; ";
}

sAlarmItem Alarm_Log::alarmItem(const sDataItem &index, uchar value)
{
    sAlarmItem it; it.alarm_status = tr("本机"); it.addr = index.addr;
    if(index.addr) it.alarm_status = tr("副机%1").arg(index.addr);
    //if(value) it.alarm_status += tr("告警"); else it.alarm_status += tr("恢复正常");
    if(index.type) it.alarm_status += tr("第%１").arg(index.id+1);
    it.alarm_status += alarmType(index) +"; ";

    if(index.topic == DTopic::Relay) {
        it.alarm_content = alarmRelay(value);
    }else if(index.type == DType::Sensor) {
        it.alarm_content = alarmSensor(value);
    }else {
        it.alarm_status += alarmStatus(value);
        it.alarm_content = alarmContent(index);
    }
    return it;
}

void Alarm_Log::alarmSlot(const sDataItem &index, uchar value)
{
    sAlarmItem it = alarmItem(index, value);
    Log_Core::bulid()->append(it);
    //qDebug() << it.module << it.content;
}
