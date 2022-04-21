#include "alarm_Updater.h"

Alarm_Updater::Alarm_Updater(QObject *parent)
    : QObject{parent}
{

}

Alarm_Updater *Alarm_Updater::bulid(QObject *parent)
{
    static Alarm_Updater* sington = nullptr;
    if(sington == nullptr) {
        sington = new Alarm_Updater(parent);
    }
    return sington;
}

bool Alarm_Updater::upRelayUnit(sAlarmIndex &index, sRelayUnit &it)
{
    bool ret = false;
    for(int i=0; i<it.size; ++i) {
        uchar alarm = 0;
        uchar value = it.sw[i];
        uchar state = it.mode[i];

        if(state == sRelay::NormaOpen) {
            if(value == sRelay::Off) alarm = sRelay::CloseAlarm;
        } else if(state == sRelay::NormaClose){
            if(value == sRelay::On) alarm = sRelay::OpenALarm;
        } else alarm = sRelay::NoAlarm; index.id = i;
        if(it.alarm[i] != alarm) emit alarmSig(index, alarm);
        it.alarm[i] = alarm; ret |= alarm;
    }    

    return ret;
}

bool Alarm_Updater::upAlarmUnit(sAlarmIndex &index, sAlarmUnit &it)
{
    bool ret = false;
    for(int i=0; i<it.size; ++i) {
        uint value = it.value[i];
        index.id = i; uchar alarm = AlarmType::Ok;
        if(it.max[i]) if(value > it.max[i]) alarm += AlarmType::Max;
        if(it.crMax[i]) if(value > it.crMax[i]) alarm += AlarmType::CrMax;
        if(it.crMin[i]) if(value < it.crMin[i]) alarm += AlarmType::CrMin;
        if(it.min[i]) if(value < it.min[i]) alarm += AlarmType::Min;
        if(it.alarm[i] != alarm) emit alarmSig(index, alarm);
        it.alarm[i] = alarm; ret |= alarm;
    }

    return ret;
}

bool Alarm_Updater::upObjData(sAlarmIndex &index, sObjData &it)
{
    bool ret = false;
    index.subtopic = AlarmIndex::Vol;
    ret |= upAlarmUnit(index, it.vol);

    index.subtopic = AlarmIndex::Cur;
    ret |= upAlarmUnit(index, it.cur);

    index.subtopic = AlarmIndex::Pow;
    ret |= upAlarmUnit(index, it.pow);

    index.subtopic = AlarmIndex::Relay;
    ret |= upRelayUnit(index, it.relay);

    return ret;
}

uchar Alarm_Updater::upTgUnit(sAlarmIndex &index, sTgUnit &it)
{
    bool ret = false;
    uint value = it.value; uchar alarm;
    if(value > it.max) alarm = AlarmType::Max;
    else if(value > it.crMax) alarm = AlarmType::CrMax;
    else if(value < it.crMin) alarm = AlarmType::CrMin;
    else if(value < it.min) alarm = AlarmType::Min;
    else {alarm = AlarmType::Ok;} index.id = 0xFF;
    if(it.alarm != alarm) emit alarmSig(index, alarm);
    it.alarm = alarm; ret |= alarm;
    return ret;
}

bool Alarm_Updater::upTgObjData(sAlarmIndex &index, sTgObjData &it)
{
    bool ret = false;
    index.subtopic = AlarmIndex::Vol;
    ret |= upTgUnit(index, it.vol);

    index.subtopic = AlarmIndex::Cur;
    ret |= upTgUnit(index, it.cur);

    index.subtopic = AlarmIndex::Pow;
    ret |= upTgUnit(index, it.pow);

    return ret;
}


bool Alarm_Updater::upEnvData(sAlarmIndex &index, sEnvData &it)
{
    bool ret = false;
    index.subtopic = AlarmIndex::Tem;
    ret |= upAlarmUnit(index, it.tem);

    index.subtopic = AlarmIndex::Hum;
    ret |= upAlarmUnit(index, it.hum);

    return ret;
}

bool Alarm_Updater::upDevData(sAlarmIndex &index, sDevData *it)
{
    bool ret = false;

    index.type = AlarmIndex::Line;
    ret |= upObjData(index, it->line);

    index.type = AlarmIndex::Loop;
    ret |= upObjData(index, it->loop);

    index.type = AlarmIndex::Output;
    ret |= upObjData(index, it->output);

    index.type = AlarmIndex::Tg;
    ret |= upTgObjData(index, it->tg);

    index.type = AlarmIndex::Env;
    ret |= upObjData(index, it->output);

    return ret;
}

bool Alarm_Updater::upDevAlarm(uchar addr)
{
    bool ret = false;
    sDevData *dev = cm::devData(addr);
    sAlarmIndex index; index.addr = addr;
    if(dev->offLine) {
        ret = upDevData(index, dev);
        dev->alarm = ret ? 1:0;
    }
    return ret;
}

void Alarm_Updater::run()
{
    int num = cm::masterDev()->info.slaveNum;
    for(int i=0; i<num+1; ++i) upDevAlarm(i);
}
