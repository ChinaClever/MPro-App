/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "alarm_object.h"

Alarm_Object::Alarm_Object()
{

}

char Alarm_Object::alarmValue(const sDIndex &index)
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

char Alarm_Object::alarmValue(const sDIndex &index, AlarmStatus type)
{
    char alarm = alarmValue(index);
    alarm = (alarm&type) ? 1: 0;
    return alarm;
}

char Alarm_Object::alarmValue(const sDIndex &index, sRelay::Alarm type)
{
    char alarm = alarmValue(index);
    alarm = (alarm==type) ? 1: 0;
    return alarm;
}

sObjData *Alarm_Object::getObjData(const sDIndex &index)
{
    sObjData *obj = nullptr;
    sDevData *dev = cm::devData(index.addr);
    switch (index.type) {
    case DType::Line: obj = &(dev->line); break;
    case DType::Loop: obj = &(dev->loop); break;
    case DType::Output: obj = &(dev->output); break;
    }
    return obj;
}

sAlarmUnit *Alarm_Object::getAlarmUnit(const sDIndex &index, sObjData *obj)
{
    sAlarmUnit *unit = nullptr;
    switch (index.topic) {
    case DTopic::Vol: unit = &(obj->vol); break;
    case DTopic::Cur: unit = &(obj->cur); break;
    case DTopic::Pow: unit = &(obj->pow); break;
    }
    return unit;
}

sAlarmUnit *Alarm_Object::getAlarmUnit(const sDIndex &index)
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
        }
    }

    return unit;
}

sTgUnit *Alarm_Object::getTgAlarmUnit(const sDIndex &index)
{
    sTgUnit *unit = nullptr;
    sTgObjData *obj = &(cm::devData(index.addr)->tg);
    if(DType::Tg == index.type) {
        switch (index.topic) {
        case DTopic::Vol: unit = &(obj->vol); break;
        case DTopic::Cur: unit = &(obj->cur); break;
        case DTopic::Pow: unit = &(obj->pow); break;
        }
    }

    return unit;
}

sRelayUnit *Alarm_Object::getRelayUnit(const sDIndex &index)
{
    sRelayUnit *unit = nullptr;
    if(DTopic::Relay == index.topic) {
        sObjData *obj = getObjData(index);
        if(obj) unit = &(obj->relay);
    }

    return unit;
}

uint *Alarm_Object::getValueAlarmUnit(const sDIndex &index)
{
    uint *ptr = nullptr;
    sAlarmUnit *unit = getAlarmUnit(index);
    if(unit) {
        switch (index.subtopic) {
        case DSub::Value: ptr = unit->value; break;
        case DSub::Rate: ptr = unit->rated; break;
        case DSub::Alarm: ptr = unit->alarm; break;
        case DSub::VMax: ptr = unit->max; break;
        case DSub::VMin: ptr = unit->min; break;
        case DSub::VCrMin: ptr = unit->crMin; break;
        case DSub::VCrMax: ptr = unit->crMax; break;
        }
        if(ptr) ptr = &(ptr[index.id]);
    }

    return ptr;
}

uint *Alarm_Object::getValueTgAlarmUnit(const sDIndex &index)
{
    uint *ret = nullptr;
    sTgUnit *unit = getTgAlarmUnit(index);
    if(unit) {
        switch (index.subtopic) {
        case DSub::Value: ret = &(unit->value); break;
        case DSub::Rate: ret = &(unit->rated); break;
        case DSub::Alarm: ret = &(unit->alarm); break;
        case DSub::VMax: ret = &(unit->max); break;
        case DSub::VMin: ret = &(unit->min); break;
        case DSub::VCrMin: ret = &(unit->crMin); break;
        case DSub::VCrMax: ret = &(unit->crMax); break;
        }
    }
    return ret;
}

uint *Alarm_Object::getValueRelayUnit(const sDIndex &index)
{
    uint *ptr = nullptr;
    sRelayUnit *unit = getRelayUnit(index);
    if(unit) {
        switch (index.subtopic) {
        case DSub::Value: ptr = unit->sw; break;
        case DSub::Rate: ptr = unit->mode; break;
        case DSub::Alarm: ptr = unit->alarm; break;
        case DSub::VMax: ptr = unit->delay; break;
        }
        if(ptr) ptr = &(ptr[index.id]);
    }
    return ptr;
}

uint *Alarm_Object::getValue(const sDIndex &index)
{
    uint *ret = nullptr;
    if(DType::Tg == index.type) ret = getValueTgAlarmUnit(index);
    else if(DTopic::Relay == index.topic) ret = getValueRelayUnit(index);
    else ret = getValueAlarmUnit(index);

    return ret;
}
