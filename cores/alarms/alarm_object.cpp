/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "alarm_object.h"
#include "op_core.h"

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

char Alarm_Object::alarmValue(const sDataItem &index, AlarmCode type)
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
    case DType::Group: obj = &(dev->group); break;
    case DType::Output: obj = &(dev->output); break;
    case DType::Dual: obj = &(dev->dual); break;
    default: cout << index.type; break;
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
    default: cout << index.topic; break;
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
        default: cout << index.topic; break;
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
        default: cout << index.topic; break;
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

void Alarm_Object::setAll(uint *ptr, uint value, int size)
{
     if(0 == size) size = OUTPUT_NUM;
     for(int i=0; i<size; ++i)  ptr[i] = value;
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
        case DSub::DPeak: ptr = unit->peakMax; break;
        case DSub::DStamp: ptr = unit->peakStamp; break;
        case DSub::DHda: ptr = unit->hda; break;
        default: ret = false; cout << index.subtopic; break;
        }
    }

    if(ptr) {
        if(index.rw){
            if(index.id) ptr[index.id-1] = index.value;
            else setAll(ptr, index.value, unit->size);
        } else index.value = ptr[index.id];

        //if((index.type == DType::Env) && (index.topic == DTopic::Tem) ) {
        //   qDebug() << index.type << index.topic << index.subtopic << index.id << index.value;
        //}
        //         if((index.type == DType::Output) && (index.topic == DTopic::Pow) ) {
        //            qDebug() << index.addr << index.type << index.topic << index.subtopic << index.id << index.value;
        //         }
    } else if(!ret)
        cout  << index.type << index.topic << index.subtopic << index.id << index.value;

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
        case DSub::Rated: ptr = unit->offAlarm; break;
        case DSub::Alarm: ptr = unit->alarm; break;
        case DSub::UpDelay: ptr = unit->powerUpDelay; break;
        case DSub::ResetDelay: ptr = unit->resetDelay; break;
        case DSub::OverrunOff: ptr = unit->overrunOff; break;
        case DSub::TimingEn: ptr = unit->timingEn; break;
        case DSub::RelayCnt: ptr = unit->cnt; break;
        case DSub::RelayEn: ptr = unit->en; break;
        case DSub::DStamp: case DSub::DHda: break;
        default: ret = false; break;
        }
    }

    if(ptr) {
        if(index.rw){
            if(index.id) ptr[index.id-1] = index.value;
            else setAll(ptr, index.value, unit->size);
        } else index.value = ptr[index.id];
    }else if(!ret) cout  << index.type << index.topic << index.subtopic << index.id << index.value;

    return ret;
}

bool Alarm_Object::sensorValue(sDataItem &index)
{    
    sEnvData *env = &(cm::devData(index.addr)->env);
    bool ret = true; switch (index.topic) {
    case DTopic::Door1: index.value = env->door[0]; break;
    case DTopic::Door2: index.value = env->door[1]; break;
    case DTopic::Water: index.value = env->water[0]; break;
    case DTopic::Smoke: index.value = env->smoke[0]; break;
    case DTopic::Wind: index.value = env->wind[0]; break;
    default: ret = false; cout << index.topic; break;
    }

    return ret;
}

bool Alarm_Object::powPfValue(sDataItem &index)
{
    bool ret = true; uint *ptr = nullptr;
    sObjData *obj = getObjData(index);
    switch (index.topic) {
    case DTopic::PF: ptr = obj->pf; break;
    case DTopic::ArtPow: ptr = obj->artPow; break;
    case DTopic::ReactivePow: ptr = obj->reactivePow; break;
    default: ret = false; cout << index.topic; break;
    }

    if(ptr) index.value = ptr[index.id];
    return ret;
}

bool Alarm_Object::eleValue(sDataItem &index)
{
    bool ret = true; uint *ptr = nullptr;
    sObjData *obj = getObjData(index);
    switch (index.topic) {
    case DTopic::Ele: ptr = obj->ele; break;
    case DTopic::HdaEle: ptr = obj->hdaEle; break;
    default: ret = false; cout << index.topic; break;
    }

    if(ptr) {
        if(index.rw){
            if(index.id) ptr[index.id-1] = 0;
            else setAll(ptr, index.value, obj->size);
            if(DTopic::Ele == index.topic) OP_Core::bulid()->clearEle(index.id);
        } else index.value = ptr[index.id];
    }
    return ret;
}

bool Alarm_Object::tgValue(sDataItem &index)
{
    bool ret = true;
    if(index.topic > DTopic::Pow) {
        sTgObjData *tg = &(cm::devData(index.addr)->tg);
        switch (index.topic) {
        case DTopic::PF: index.value = tg->pf; break;
        case DTopic::Ele: index.value = tg->ele; break;
        case DTopic::ArtPow: index.value = tg->artPow; break;
        case DTopic::ReactivePow: index.value = tg->reactivePow; break;
        default: ret = false; cout << index.topic; break;
        }
    } else ret = tgAlarmUnitValue(index);
    return ret;
}

bool Alarm_Object::upMetaData(sDataItem &index)
{
    bool  ret = false; uchar addr = index.addr;
    if(addr >= 0xff) index.addr = 0;

    if(index.addr > DEV_NUM) {cout << index.addr; return ret;}
    switch (index.type) {
    case DType::Tg: return tgValue(index);
    case DType::Sensor: return sensorValue(index);
    }

    switch (index.topic) {
    case DTopic::HdaEle: case DTopic::Ele: ret = eleValue(index); break;
    case DTopic::Relay: ret = relayUnitValue(index); break;
    case DTopic::ArtPow: case DTopic::ReactivePow:
    case DTopic::PF: ret = powPfValue(index); break;
    default: ret = alarmUnitValue(index); break;
    } index.addr = addr;

    return ret;
}
