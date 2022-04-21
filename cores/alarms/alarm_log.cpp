#include "alarm_log.h"
#include "log_core.h"

Alarm_Log::Alarm_Log(QObject *parent)
    : QObject{parent}
{
    qRegisterMetaType<sAlarmIndex>("sAlarmIndex");
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


QString Alarm_Log::alarmType(const sAlarmIndex &index)
{
    QString str;
    if(index.type) str = tr("第%１ ").arg(index.id+1);
    switch (index.type) {
    case AlarmIndex::Tg: str += tr("总"); break;
    case AlarmIndex::Line: str += tr("相"); break;
    case AlarmIndex::Loop: str += tr("回路"); break;
    case AlarmIndex::Output: str += tr("输出位"); break;
    case AlarmIndex::Env: str += tr("传感器"); break;
    }

    switch (index.subtopic) {
    case AlarmIndex::Vol: str += tr("电压"); break;
    case AlarmIndex::Cur: str += tr("电流"); break;
    case AlarmIndex::Pow: str += tr("功率"); break;
    case AlarmIndex::Tem: str += tr("温度"); break;
    case AlarmIndex::Hum: str += tr("湿度"); break;
    case AlarmIndex::Relay: str += tr("开关"); break;
    }

    return str;
}

QString Alarm_Log::alarmStatus(uchar value, QString &state)
{
    QString str; switch (value) {
    case AlarmType::Ok: state = str = tr("恢复正常"); break;
    case AlarmType::Min: state = tr("告警"); str = tr("过低"); break;
    case AlarmType::CrMin: state = tr("预警"); str = tr("过低"); break;
    case AlarmType::CrMax: state = tr("预警"); str = tr("过高"); break;
    case AlarmType::Max: state = tr("告警"); str = tr("过高"); break;
    }
    return str;
}

QString Alarm_Log::alarmContent(const sAlarmIndex &index)
{
    QString str;
    double rate = 1;
    int id = index.id;

    switch (index.subtopic) {
    case AlarmIndex::Vol: rate = COM_RATE_VOL; break;
    case AlarmIndex::Cur: rate = COM_RATE_CUR; break;
    case AlarmIndex::Pow: rate = COM_RATE_POW; break;
    case AlarmIndex::Tem: rate = COM_RATE_TEM; break;
    case AlarmIndex::Hum: rate = COM_RATE_HUM; break;
    }

    Alarm_Object obj;
    if(index.type) {
        sAlarmUnit *unit = obj.getAlarmUnit(index);
        if(unit) {
            str  = tr("当前值=%1　告警最小值=%2 预警最小值=%3 预警最大值=%4 告警最大值=%5")
                    .arg(unit->value[id]/rate)
                    .arg(unit->min[id] / 10.0)
                    .arg(unit->crMin[id] / 10.0)
                    .arg(unit->crMax[id] / 10.0)
                    .arg(unit->max[id] / 10.0);
        } else qDebug() << Q_FUNC_INFO;
    } else {
        sTgUnit *unit = obj.getTgAlarmUnit(index);
        if(unit) {
            str  = tr("当前值=%1　告警最小值=%2 预警最小值=%3 预警最大值=%4 告警最大值=%5")
                    .arg(unit->value/rate)
                    .arg(unit->min / 10.0)
                    .arg(unit->crMin / 10.0)
                    .arg(unit->crMax / 10.0)
                    .arg(unit->max / 10.0);
        } else qDebug() << Q_FUNC_INFO;
    }
    return str;
}


void Alarm_Log::alarmRelay(const sAlarmIndex &index, uchar value)
{
    sAlarmItem it; it.addr = tr("本机");
    if(index.addr) it.addr = tr("副机 %1").arg(index.addr);
    if(value) it.state = tr("告警"); else it.state =tr("恢复正常");

    QString str = tr("断路器");
    if(index.type == AlarmIndex::Output) str = tr("继电器");
    str += QString::number(index.id+1);
    it.module = str;

    switch (value) {
    case sRelay::CloseAlarm: str = tr("断开"); break;
    case sRelay::OpenALarm: str = tr("闭合"); break;
    case sRelay::NoAlarm: str = tr("恢复"); break;
    } it.content = str;
    Log_Core::bulid()->append(it);
}


void Alarm_Log::alarmSlot(sAlarmIndex index, uchar value)
{
    if(index.subtopic != AlarmIndex::Relay) {
        sAlarmItem it; it.addr = tr("本机");
        if(index.addr) it.addr = tr("副机 %1").arg(index.addr);
        it.module = alarmType(index); it.module += alarmStatus(value, it.state);
        it.content = alarmContent(index);
        Log_Core::bulid()->append(it);
    } else {
        alarmRelay(index, value);
    }
}
