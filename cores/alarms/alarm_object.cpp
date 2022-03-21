#include "alarm_object.h"

Alarm_Object::Alarm_Object()
{

}

char Alarm_Object::alarmValue(const sAlarmIndex &index)
{
    char alarm = -1;
    if(index.type) {
        if(AlarmIndex::Relay == index.subtopic) {
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

char Alarm_Object::alarmValue(const sAlarmIndex &index, AlarmType type)
{
    char alarm = alarmValue(index);
    alarm = (alarm&type) ? 1: 0;
    return alarm;
}

char Alarm_Object::alarmValue(const sAlarmIndex &index, sRelay::Alarm type)
{
    char alarm = alarmValue(index);
    alarm = (alarm==type) ? 1: 0;
    return alarm;
}

sObjData *Alarm_Object::getObjData(const sAlarmIndex &index)
{
    sObjData *obj = nullptr;
    sDevData *dev = cm::devData(index.addr);
    switch (index.type) {
    case AlarmIndex::Line: obj = &(dev->line); break;
    case AlarmIndex::Loop: obj = &(dev->loop); break;
    case AlarmIndex::Output: obj = &(dev->output); break;
    }
    return obj;
}

sAlarmUnit *Alarm_Object::getAlarmUnit(const sAlarmIndex &index, sObjData *obj)
{
    sAlarmUnit *unit = nullptr;
    switch (index.subtopic) {
    case AlarmIndex::Vol: unit = &(obj->vol); break;
    case AlarmIndex::Cur: unit = &(obj->cur); break;
    case AlarmIndex::Pow: unit = &(obj->pow); break;
    }
    return unit;
}

sAlarmUnit *Alarm_Object::getAlarmUnit(const sAlarmIndex &index)
{
    sObjData *obj = nullptr; sAlarmUnit *unit = nullptr;
    if(AlarmIndex::Tg == index.type) return unit;
    sDevData *dev = cm::devData(index.addr);

    if(index.type < AlarmIndex::Env) {
        obj = getObjData(index);
        unit = getAlarmUnit(index, obj);
    } else {
        switch (index.type) {
        case AlarmIndex::Tem: unit = &(dev->env.tem);break;
        case AlarmIndex::Hum: unit = &(dev->env.hum);break;
        }
    }

    return unit;
}

sTgUnit *Alarm_Object::getTgAlarmUnit(const sAlarmIndex &index)
{
    sTgUnit *unit = nullptr;
    sTgObjData *obj = &(cm::devData(index.addr)->tg);
    if(AlarmIndex::Tg == index.type) {
        switch (index.subtopic) {
        case AlarmIndex::Vol: unit = &(obj->vol); break;
        case AlarmIndex::Cur: unit = &(obj->cur); break;
        case AlarmIndex::Pow: unit = &(obj->pow); break;
        }
    }

    return unit;
}

sRelayUnit *Alarm_Object::getRelayUnit(const sAlarmIndex &index)
{
    sRelayUnit *unit = nullptr;
    if(AlarmIndex::Relay == index.subtopic) {
        sObjData *obj = getObjData(index);
        if(obj) unit = &(obj->relay);
    }

    return unit;
}
