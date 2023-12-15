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
/**
 * 获取当前告警
 */
QString Alarm_Log::getCurrentAlarm(int addr)
{
    QString res; if(addr) { /*为副机*/
        res = m_currentAlarm[addr-1];   /*获取副机告警信息*/
    } else {    /*为主机则获取所有设备的当前告警信息信息*/
        for(int i=0; i<DEV_NUM; ++i) {
            if(m_currentAlarm[i].size())
                res += m_currentAlarm[i];   /*判断如果有告警则添加到res书组*/
        }
    }
    return res; /*返回存储告警信息的res变量*/
}

void Alarm_Log::appendAlarm(const sDataItem &index, uchar value)
{
    sAlarmItem it = alarmItem(index, value);
    //QString str = QString::number(++m_id) +"、";
    QString str = it.alarm_status + it.alarm_content;   /*获取告警状态和告警内容*/
    if(str.size()) m_currentAlarm[it.addr] += str + "\n";   /*如果告警信息存在，将告警内容和告警状态添加到 m_currentAlarm 变量*/
    //cout << m_currentAlarm[it.addr].size() << str;
}

void Alarm_Log::appendSlaveOffline(int addr)
{
    QString str = tr("副机%1离线;").arg(addr);
    if(!cm::cn()) str = tr("slave %1 off-line;").arg(addr);
    m_currentAlarm[0] += str + "\n";
}

void Alarm_Log::resetAwtk()
{
    system("killall awtk");
    system("awtk &");
    //cout << "awtk";
}
/**
 * 根据当前告警记录生成二维码，先从告警记录中提取数据，如果提取的数据为空，则使用主设备配置中的二维码数据，
 * 如果提取的数据不为空，则更新二维码数据并生成新的二维码
 */
void Alarm_Log::generateQRcode()
{
    static QString alarm=" "; QString str = m_currentAlarm[0];
    if(str.size()) str = str.split("\n").first().split(";").first();    /*判断警告信息是否存在*/
    if(str.isEmpty()) str = cm::masterDev()->cfg.uut.qrcode; //cout << str << alarm;
    if((str != alarm)) { alarm = str; cm::qrcodeGenerator(str); resetAwtk();}
}
/**
 * 根据传入的type和topic，返回对应的告警类型和字符串
 */
QString Alarm_Log::alarmType(const sDataItem &index)
{
    QString str;

    switch (index.type) {
    case DType::Tg: if(cm::cn()) str += tr("总"); else str += "assemble"; break;
    case DType::Line: if(cm::cn()) str += tr("相"); else str += "phase "; break;
    case DType::Loop: if(cm::cn()) str += tr("回路"); else str += "loop "; break;
    case DType::Group: if(cm::cn()) str += tr("组"); else str += "group "; break;
    case DType::Dual: if(cm::cn()) str += tr("双电源"); else str += "rack "; break;
    case DType::Output: if(cm::cn()) str += tr("输出位"); else str += "outlet "; break;
    case DType::Env: if(cm::cn()) str += tr("环境"); else str += "environment "; break;
    case DType::Sensor: if(cm::cn()) str += tr("传感器"); else str += "sensor "; break;
    case DType::CabTg: if(cm::cn()) str += tr("机柜总"); else str += "cabinet assembly "; break;
    case DType::CabLine: if(cm::cn()) str += tr("机柜相"); else str += "cabinet phase "; break;
    case DType::CabLoop: if(cm::cn()) str += tr("机柜回路"); else str += "cabinet circuit "; break;
    }

    switch (index.topic) {
    case DTopic::Vol: if(cm::cn()) str += tr("电压"); else str += "voltage "; break;
    case DTopic::Cur: if(cm::cn()) str += tr("电流"); else str += "current "; break;
    case DTopic::Pow: if(cm::cn()) str += tr("功率"); else str += "active power "; break;
    case DTopic::Ele: if(cm::cn()) str += tr("电能"); else str += "electric energy "; break;
    case DTopic::Tem: if(cm::cn()) str += tr("温度"); else str += "temperature "; break;
    case DTopic::Hum: if(cm::cn()) str += tr("湿度"); else str += "humidity "; break;
    case DTopic::Door1: if(cm::cn()) str += tr("门禁1"); else str += "door 1 "; break;
    case DTopic::Door2: if(cm::cn()) str += tr("门禁２"); else str += "door 2 "; break;
    case DTopic::Water: if(cm::cn()) str += tr("水浸"); else str += "water "; break;
    case DTopic::Smoke: if(cm::cn()) str += tr("烟雾"); else str += "smog "; break;
    case DTopic::Relay:
        if(cm::cn()) {if(index.type == DType::Loop) str += tr("断路器"); else str += tr("开关");}
        else {if(index.type == DType::Loop) str += "circuit breaker "; else str += "relay";} break;
    }

    return str;
}

QString Alarm_Log::alarmStatus(uchar value)
{
    QString state; switch (value) {
    case AlarmCode::Ok: if(cm::cn()) state = tr("恢复正常"); else state = "return to normal "; break;
    case AlarmCode::Min: if(cm::cn()) state = tr("过低告警"); else state = "under voltage alarm "; break;
    case AlarmCode::CrMin: if(cm::cn()) state = tr("过低预警"); else state = "low warning "; break;
    case AlarmCode::CrMax: if(cm::cn()) state = tr("过高预警"); else state = "excessive warning "; break;
    case AlarmCode::Max: if(cm::cn()) state = tr("过高告警");  else state = "over high alarm "; break;
    }
    return state+"; ";
}

QString Alarm_Log::alarmContent(const sDataItem &index)
{
    double rate = 1;
    int id = index.id;
    QString str, suffix;
    int offset = 0;

    switch (index.topic) {
    case DTopic::Vol: rate = COM_RATE_VOL; suffix = "V"; break;
    case DTopic::Cur: rate = COM_RATE_CUR; suffix = "A"; break;
    case DTopic::Pow: rate = COM_RATE_POW; suffix = "KW"; break;
    case DTopic::Tem: rate = COM_RATE_TEM; suffix = "°C";  offset=40; break;
    case DTopic::Hum: rate = COM_RATE_HUM; suffix = "%";break;
    } if(DTopic::Tem == index.topic) offset= 40;


    Alarm_Object obj;
    if(index.type) {
        sAlarmUnit *unit = obj.getAlarmUnit(index);
        double value = unit->value[id]/rate; if(value > 0) value -= offset;
        if(unit) {
            if(cm::cn()) str = tr("当前值:%1%6　告警最小值:%2%6 预警最小值:%3%6 预警最大值:%4%6 告警最大值:%5%6");
            else str = "Current value:%1%6 Alarm minimum value:%2%6, early warning minimum value:%3%6 "
                        "early warning maximum value:%4%6 Alarm maximum value:%5%6";
            str  = str.arg(value)
                    .arg(unit->min[id] / rate  - offset)
                    .arg(unit->crMin[id] / rate - offset)
                    .arg(unit->crMax[id] / rate - offset)
                    .arg(unit->max[id] / rate - offset)
                    .arg(suffix);
        } else qDebug() << Q_FUNC_INFO;
    } else {
        sTgUnit *unit = obj.getTgAlarmUnit(index);
        if(unit) {
            if(cm::cn()) str = tr("当前值:%1%6　告警最小值:%2%6 预警最小值:%3%6 预警最大值:%4%6 告警最大值:%5%6");
            else str = "Current value:%1%6 Alarm minimum value:%2%6, early warning minimum value:%3%6 "
                       "early warning maximum value:%4%6 Alarm maximum value:%5%6";
            str = str.arg(unit->value/rate)
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
    QString str; switch (value) {
    case sRelay::OffALarm: if(cm::cn()) str = tr("断开"); else str = "break "; break;
    case sRelay::NoAlarm: if(cm::cn()) str = tr("恢复"); else str = "restore "; break;
    case sRelay::LifeAlarm: if(cm::cn()) str = tr("寿命"); else str = "life "; break;
    }
    return str+"; ";
}

QString Alarm_Log::alarmSensor(uchar value)
{
    QString str = tr("恢复正常"); if(!cm::cn()) str = "return to normal ";
    if(value){ if(cm::cn()) str = tr("告警");else str = "alarm "; }
    return str+"; ";
}

sAlarmItem Alarm_Log::alarmItem(const sDataItem &index, uchar value)
{
    sAlarmItem it;  it.addr = index.addr;
    if(cm::cn()) {
        it.alarm_status = tr("本机");
        if(index.addr) it.alarm_status = tr("副机%1").arg(index.addr);
        if(index.type) it.alarm_status += tr("第%１").arg(index.id+1);
    } else {
        it.alarm_status = "Native ";
        if(index.addr) it.alarm_status = tr("Slave %1 ").arg(index.addr);
        if(index.type) it.alarm_status += tr("%１").arg(index.id+1);
    } it.alarm_status += alarmType(index) +", ";

    if(index.topic == DTopic::Relay) {  /*开关*/
        it.alarm_content = alarmRelay(value);   /*告警内容赋值给it.alarm_content变量*/
#if 0
         if(DType::Loop == index.type) {
            Alarm_Object obj;
            sDataItem item = index;
            item.topic = DTopic::Vol;
            sAlarmUnit *unit = obj.getAlarmUnit(item);
            double v = unit->value[index.id] /COM_RATE_VOL;
            it.alarm_content += tr("vol = %1V").arg(v);
         }
#endif
    }else if(index.type == DType::Sensor) { /*传感器*/
        it.alarm_content = alarmSensor(value);  /*告警内容赋值给it.alarm_content变量*/
    }else {
        it.alarm_status += alarmStatus(value);  /*告警内容赋值给it.alarm_status变量*/
        it.alarm_content = alarmContent(index); /*告警内容赋值给it.alarm_content变量*/
    }
    return it;
}

void Alarm_Log::alarmSlot(const sDataItem &index, uchar value)
{
    sAlarmItem it = alarmItem(index, value);    /*告警项目*/
    Log_Core::bulid()->append(it);
    //qDebug() << it.module << it.content;
}
