/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "alarm_Updater.h"

Alarm_Updater::Alarm_Updater(QObject *parent)
    : QObject{parent}
{
    qRegisterMetaType<sDataItem>("sDataItem");
}

Alarm_Updater *Alarm_Updater::bulid(QObject *parent)
{
    static Alarm_Updater* sington = nullptr;
    if(sington == nullptr) {
        sington = new Alarm_Updater(parent);
    }
    return sington;
}

bool Alarm_Updater::upRelayUnit(sDataItem &index, sRelayUnit &it)
{
    bool ret = false;
    for(int i=0; i<it.size; ++i) {
        uchar alarm = 0;
        uchar value = it.sw[i];
        uchar state = it.offAlarm[i];
        if(state == sRelay::EnOffALarm) {
            if(value == sRelay::Off) alarm = sRelay::OffALarm;
        } else alarm = sRelay::NoAlarm; index.id = i;
        if(it.alarm[i] != alarm) emit alarmSig(index, alarm);
        it.alarm[i] = alarm; ret |= alarm;
    }

    return ret;
}

bool Alarm_Updater::upAlarmItem(sDataItem &index, int i, sAlarmUnit &it)
{
    bool ret = false;
    uint value = it.value[i];
    index.id = i; uchar alarm = AlarmCode::Ok;
    if(value > it.max[i]) alarm = AlarmCode::Max;
    if(value > it.crMax[i]) alarm = AlarmCode::CrMax;
    if(value < it.crMin[i]) alarm = AlarmCode::CrMin;
    if(value < it.min[i]) alarm = AlarmCode::Min;

    if(it.alarm[i] != alarm)  {
        if(++it.cnt[i] > 3) {
            emit alarmSig(index, alarm);
            it.alarm[i] = alarm;
            ret |= alarm;
        }
    } else it.cnt[i] = 0;

    return ret;
}

bool Alarm_Updater::upAlarmUnit(sDataItem &index, sAlarmUnit &it)
{
    bool ret = false;
    for(int i=0; i<it.size; ++i) {
        if(it.en[i]) {
            ret |= upAlarmItem(index, i, it);
        } else it.alarm[i] = AlarmCode::Ok;
    }

    return ret;
}

bool Alarm_Updater::upObjData(sDataItem &index, sObjData &it)
{
    bool ret = false;
    index.topic = DTopic::Vol;
    ret |= upAlarmUnit(index, it.vol);

    index.topic = DTopic::Cur;
    ret |= upAlarmUnit(index, it.cur);

    index.topic = DTopic::Pow;
    ret |= upAlarmUnit(index, it.pow);

    index.topic = DTopic::Relay;
    ret |= upRelayUnit(index, it.relay);

    return ret;
}

uchar Alarm_Updater::upTgUnit(sDataItem &index, sTgUnit &it)
{
    bool ret = false; if(it.en) {
        uint value = it.value; uchar alarm;
        if(value > it.max) alarm = AlarmCode::Max;
        else if(value > it.crMax) alarm = AlarmCode::CrMax;
        else if(value < it.crMin) alarm = AlarmCode::CrMin;
        else if(value < it.min) alarm = AlarmCode::Min;
        else {alarm = AlarmCode::Ok;} index.id = 0;
        if(it.alarm != alarm) emit alarmSig(index, alarm);
        it.alarm = alarm; ret |= alarm;
    }
    return ret;
}

bool Alarm_Updater::upTgObjData(sDataItem &index, sTgObjData &it)
{
    bool ret = false;
    index.topic = DTopic::Vol;
    ret |= upTgUnit(index, it.vol);

    index.topic = DTopic::Cur;
    ret |= upTgUnit(index, it.cur);

    index.topic = DTopic::Pow;
    ret |= upTgUnit(index, it.pow);

    return ret;
}


bool Alarm_Updater::upEnvData(sDataItem &index, sEnvData &it)
{
    bool ret = false;
    index.topic = DTopic::Tem;
    //ret |= upAlarmUnit(index, it.tem);
    for(int i=0; i<SENOR_NUM; ++i) {
        if(it.tem.en[i] && it.isInsert[i]) {
            ret |= upAlarmItem(index, i, it.tem);
        } else it.tem.alarm[i] = AlarmCode::Ok;
    }

    index.topic = DTopic::Hum;
    //ret |= upAlarmUnit(index, it.hum);
    for(int i=0; i<SENOR_NUM; ++i) {
        if(it.hum.en[i] && it.isInsert[i]) {
            ret |= upAlarmItem(index, i, it.hum);
        } else it.hum.alarm[i] = AlarmCode::Ok;
    }

    return ret;
}

bool Alarm_Updater::upSensorStatus(sDataItem &index, uint *ptr, int id)
{
    bool ret = false; if(ptr[id]) {
        uchar alarm = 0; index.id = id;
        if(ptr[id] == 2) alarm = 1; else alarm = 0;
        if(ptr[id+2] != alarm) emit alarmSig(index, alarm);
        ptr[id+2] = alarm; ret |= alarm;
    }
    return ret;
}

bool Alarm_Updater::upSensors(sDataItem &index, sEnvData &it)
{
    bool ret = false;
    index.topic = DTopic::Door1;
    ret |=  upSensorStatus(index, it.door);

    index.topic = DTopic::Door2;
    ret |=  upSensorStatus(index, it.door,1);

    index.topic = DTopic::Water;
    ret |=  upSensorStatus(index, it.water);

    index.topic = DTopic::Smoke;
    ret |=  upSensorStatus(index, it.smoke);

    return ret;
}

bool Alarm_Updater::upDevData(sDataItem &index, sDevData *it)
{
    bool ret = false;

    index.type = DType::Line;
    ret |= upObjData(index, it->line);

    index.type = DType::Loop;
    ret |= upObjData(index, it->loop);

    index.type = DType::Output;
    ret |= upObjData(index, it->output);

    index.type = DType::Group;
    ret |= upObjData(index, it->group);

    index.type = DType::Dual;
    ret |= upObjData(index, it->dual);

    index.type = DType::Tg;
    ret |= upTgObjData(index, it->tg);

    index.type = DType::Env;
    ret |= upEnvData(index, it->env);

    index.type = DType::Sensor;
    ret |= upSensors(index, it->env);

    return ret;
}

bool Alarm_Updater::upDevAlarm(uchar addr)
{
    bool ret = false;
    sDevData *dev = cm::devData(addr);
    sDataItem index; index.addr = addr;
    if(dev->offLine || addr==0) {
        ret = upDevData(index, dev);
        dev->alarm = ret ? 1:0;
        dev->status = dev->alarm;
        if(dev->dtc.fault) dev->status = 2;
    }
    return ret;
}

void Alarm_Updater::run()
{
    int num = cm::masterDev()->cfg.nums.slaveNum;
    for(int i=0; i<num+1; ++i) upDevAlarm(i);
}
