/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "alarm_object.h"

Alarm_Object::Alarm_Object()
{

}

char Alarm_Object::alarmValue(const sDataItem &index)
{
    char alarm = -1;
    if(index.type) {
        if(DTopic::Relay == index.topic) {
            sRelayUnit *unit = getRelayUnit(index);
            if(unit) alarm = unit->alarm[index.id];
        } else {
            sAlarmUnit *unit = getAlarmUnit(index);
            if(unit) alarm = unit->alarm[index.id];
        }
    } else {
        sTgUnit *unit = getTgAlarmUnit(index);
        if(unit) alarm = unit->alarm;
    }

    return alarm;
}

char Alarm_Object::alarmValue(const sDataItem &index, AlarmStatus type)
{
    char alarm = alarmValue(index);
    alarm = (alarm&type) ? 1: 0;
    return alarm;
}

char Alarm_Object::alarmValue(const sDataItem &index, sRelay::Alarm type)
{
    char alarm = alarmValue(index);
    alarm = (alarm==type) ? 1: 0;
    return alarm;
}

sObjData *Alarm_Object::getObjData(const sDataItem &index)
{
    sObjData *obj = nullptr;
    sDevData *dev = cm::devData(index.addr);
    switch (index.type) {
    case DType::Line: obj = &(dev->line); break;
    case DType::Loop: obj = &(dev->loop); break;
    case DType::Output: obj = &(dev->output); break;
    default: qDebug() << Q_FUNC_INFO; break;
    }
    return obj;
}

sAlarmUnit *Alarm_Object::getAlarmUnit(const sDataItem &index, sObjData *obj)
{
    sAlarmUnit *unit = nullptr;
    switch (index.topic) {
    case DTopic::Vol: unit = &(obj->vol); break;
    case DTopic::Cur: unit = &(obj->cur); break;
    case DTopic::Pow: unit = &(obj->pow); break;
    default: qDebug() << Q_FUNC_INFO; break;
    }
    return unit;
}

sAlarmUnit *Alarm_Object::getAlarmUnit(const sDataItem &index)
{
    sObjData *obj = nullptr; sAlarmUnit *unit = nullptr;
    if(DType::Tg == index.type) return unit;
    sDevData *dev = cm::devData(index.addr);

    if(index.type < DType::Env) {
        obj = getObjData(index);
        unit = getAlarmUnit(index, obj);
    } else {
        switch (index.topic) {
        case DTopic::Tem: unit = &(dev->env.tem);break;
        case DTopic::Hum: unit = &(dev->env.hum);break;
        default: qDebug() << Q_FUNC_INFO; break;
        }
    }

    return unit;
}

sTgUnit *Alarm_Object::getTgAlarmUnit(const sDataItem &index)
{
    sTgUnit *unit = nullptr;
    sTgObjData *obj = &(cm::devData(index.addr)->tg);
    if(DType::Tg == index.type) {
        switch (index.topic) {
        case DTopic::Vol: unit = &(obj->vol); break;
        case DTopic::Cur: unit = &(obj->cur); break;
        case DTopic::Pow: unit = &(obj->pow); break;
        default: qDebug() << Q_FUNC_INFO; break;
        }
    }

    return unit;
}

sRelayUnit *Alarm_Object::getRelayUnit(const sDataItem &index)
{
    sRelayUnit *unit = nullptr;
    if(DTopic::Relay == index.topic) {
        sObjData *obj = getObjData(index);
        if(obj) unit = &(obj->relay);
    }

    return unit;
}


bool Alarm_Object::alarmUnitValue(sDataItem &index)
{
    bool ret = true; uint *ptr = nullptr;
    sAlarmUnit *unit = getAlarmUnit(index);
    if(unit) {
        switch (index.subtopic) {
        case DSub::Size: index.value = unit->size; break;
        case DSub::Value: ptr = unit->value; break;
        case DSub::Rated: ptr = unit->rated; break;
        case DSub::Alarm: ptr = unit->alarm; break;
        case DSub::VMax: ptr = unit->max; break;
        case DSub::VMin: ptr = unit->min; break;
        case DSub::VCrMin: ptr = unit->crMin; break;
        case DSub::VCrMax: ptr = unit->crMax; break;
        case DSub::EnAlarm: ptr = unit->en; break;
        default: ret = false; qDebug() << Q_FUNC_INFO; break;
        }
    }

    if(ptr) {
        if(index.rw){
            if(index.id) ptr[index.id-1] = index.value;
            else for(int i=0; i<unit->size; ++i) ptr[i] = index.value;
        } else index.value = ptr[index.id];
    }

    return ret;
}

bool Alarm_Object::tgAlarmUnitValue(sDataItem &index)
{
    bool ret = true; uint *ptr = nullptr;
    sTgUnit *unit = getTgAlarmUnit(index);
    if(unit) {
        switch (index.subtopic) {
        case DSub::Size: index.value = 1; break;
        case DSub::Value: ptr = &(unit->value); break;
        case DSub::Rated: ptr = &(unit->rated); break;
        case DSub::Alarm: ptr = &(unit->alarm); break;
        case DSub::VMax: ptr = &(unit->max); break;
        case DSub::VMin: ptr = &(unit->min); break;
        case DSub::VCrMin: ptr = &(unit->crMin); break;
        case DSub::VCrMax: ptr = &(unit->crMax); break;
        case DSub::EnAlarm: ptr = &(unit->en); break;
        default: ret = false; qDebug() << Q_FUNC_INFO; break;
        }
    }

    if(ptr) {
        if(index.rw) *ptr = index.value;
        else index.value = *ptr;
    }

    return ret;
}

bool Alarm_Object::relayUnitValue(sDataItem &index)
{
    bool ret = true; uint *ptr = nullptr;
    sRelayUnit *unit = getRelayUnit(index);
    if(unit) {
        switch (index.subtopic) {
        case DSub::Size: index.value = unit->size; break;
        case DSub::Value: ptr = unit->sw; break;
        case DSub::Rated: ptr = unit->mode; break;
        case DSub::Alarm: ptr = unit->alarm; break;
        case DSub::VMax: ptr = unit->delay; break;
        default: ret = false; qDebug() << Q_FUNC_INFO; break;
        }
    }

    if(ptr) {
        if(index.rw){
            if(index.id) ptr[index.id-1] = index.value;
            else for(int i=0; i<unit->size; ++i) ptr[i] = index.value;
        } else index.value = ptr[index.id];
    }

    return ret;
}

bool Alarm_Object::sensorValue(sDataItem &index)
{
    bool ret = true;
    sEnvData *env = &(cm::devData(index.addr)->env);

    switch (index.topic) {
    case DTopic::Door1: index.value = env->door[0]; break;
    case DTopic::Door2: index.value = env->door[1]; break;
    case DTopic::Water: index.value = env->water[0]; break;
    case DTopic::Smoke: index.value = env->smoke[0]; break;
    default: ret = false; qDebug() << Q_FUNC_INFO; break;
    }

    return ret;
}

bool Alarm_Object::upIndexValue(sDataItem &index)
{
    bool  ret = false;
    if(DType::Tg == index.type) ret = tgAlarmUnitValue(index);
    else if(DTopic::Relay == index.topic) ret = relayUnitValue(index);
    else if(DType::Sensor == index.type) ret = sensorValue(index);
    else ret = alarmUnitValue(index);

    return ret;
}
