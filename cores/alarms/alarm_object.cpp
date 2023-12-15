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
/**
 * 根据传入的告警类型，获取对应的告警值
 */
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
/**
 * 根据传入的type，获取对应的对象数据，相数据、回路数据、组数据、输出位数据、双电原数据、机柜相、机柜回路
 */
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
    case DType::CabLine: obj = &(dev->cabLine); break;
    case DType::CabLoop: obj = &(dev->cabLoop); break;
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
    return unit;    /*返回获取的单位，电压、电流、功率*/
}

sAlarmUnit *Alarm_Object::getAlarmUnit(const sDataItem &index)
{
    sObjData *obj = nullptr; sAlarmUnit *unit = nullptr;
    if(DType::Tg == index.type) return unit;    /*如果type是统计数据则直接返回空*/
    sDevData *dev = cm::devData(index.addr);

    if(index.type == DType::Env) {  /*如果type是环境数据*/
        switch (index.topic) {
        case DTopic::Tem: unit = &(dev->env.tem);break;
        case DTopic::Hum: unit = &(dev->env.hum);break;
        default: cout << index.topic; break;
        }
    } else {    /*如果type不是环境数据*/
        obj = getObjData(index);
        unit = getAlarmUnit(index, obj);
    }

    return unit;
}
/**
 * 获取统计数据的告警单位，电压、电流、功率
 */
sTgUnit *Alarm_Object::getTgAlarmUnit(const sDataItem &index)
{
    sTgUnit *unit = nullptr;
    sTgObjData *obj = &(cm::devData(index.addr)->tg);
    if(DType::CabTg == index.type) obj = &(cm::devData(index.addr)->cabTg); /*机柜统计数据*/
    if((DType::Tg == index.type) || (DType::CabTg == index.type)) {/*统计数据或机柜统计数据*/
        switch (index.topic) {
        case DTopic::Vol: unit = &(obj->vol); break;
        case DTopic::Cur: unit = &(obj->cur); break;
        case DTopic::Pow: unit = &(obj->pow); break;
        default: cout << index.topic; break;
        }
    }

    return unit;   /*返回获取的单位，电压、电流、功率*/
}
/**
 * 获取开关
 */
sRelayUnit *Alarm_Object::getRelayUnit(const sDataItem &index)
{
    sRelayUnit *unit = nullptr;
    if(DTopic::Relay == index.topic) {
        sObjData *obj = getObjData(index);
        if(obj) unit = &(obj->relay);
    }

    return unit;
}

bool Alarm_Object::setAll(uint *ptr, sDataItem &index, sAlarmUnit *unit)
{
    bool ret =  true;
    int size = unit->size;
    uint value = index.value;
    for(int i=0; i<size; ++i) {
        sDataItem it = index; it.id = i+1;
        ret = alarmUnitCheck(it, unit); /*检查每一个id的值是否符合要求*/
        if(!ret) return ret;
    }

    return setAll(ptr, value, size);
}

bool Alarm_Object::setAll(uint *ptr, uint value, int size)
{
    if(0 == size) size = OUTPUT_NUM;
    for(int i=0; i<size; ++i)  ptr[i] = value;  /*全部设置*/
    return true;
}
/**
 * 检查告警值是否符合要求，不符合则返回false
 */
bool Alarm_Object::alarmUnitCheck(sDataItem &index, sAlarmUnit *unit)
{
    bool ret = true; int id = index.id; if(id) id -= 1;
    if(index.txType == DTxType::TxWeb) return ret;  /*通讯类型为网页则直接返回true*/
    uint v = index.value; switch (index.subtopic) {
    case DSub::VMax: if((v > (unit->rated[id])*1.3) || (v < unit->crMax[id])) ret = false; break;
    case DSub::VCrMax: if((v > unit->max[id]) || (v < unit->crMin[id])) ret = false; break;
    case DSub::VCrMin: if((v > unit->crMax[id]) || (v < unit->min[id])) ret = false; break;
    case DSub::VMin: if(v > unit->crMax[id]) ret = false; break;
    } if(id >= unit->size) ret = false;

    return ret;
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

    if(index.rw && (DSub::EnAlarm == index.subtopic)) {
        if(index.value > 1) index.value = 1;
    }

    if(ptr) {
        if(index.rw){
            if(index.id) {
                ret = alarmUnitCheck(index, unit);
                if(ret) ptr[index.id-1] = index.value;
            } else ret = setAll(ptr, index, unit);
        } else index.value = ptr[index.id];

        //if((index.type == DType::Env) && (index.topic == DTopic::Tem) ) {
        //   qDebug() << index.type << index.topic << index.subtopic << index.id << index.value;
        //}
        //         if((index.type == DType::Output) && (index.topic == DTopic::Pow) ) {
        //            qDebug() << index.addr << index.type << index.topic << index.subtopic << index.id << index.value;
        //         }
    } else if(!ret) cout  << index.type << index.topic << index.subtopic << index.id << index.value << ret;

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
        case DSub::RelayEn: ptr = unit->disabled; break;
        case DSub::DStamp: ptr = unit->maxCnt; break;
        case DSub::RelayLifeEn: ptr = unit->lifeEn; break;
        default: ret = false; break;
        }
    }

    if(ptr) {
        if(index.rw){
            if(index.id) ptr[index.id-1] = index.value;
            else setAll(ptr, index.value, unit->size);
        } else index.value = ptr[index.id];
    }else if(!ret) cout << index.type << index.topic << index.subtopic << index.id << index.value;

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
    case DTopic::LineVol: ptr = obj->lineVol; break;
    case DTopic::ReactivePow: ptr = obj->reactivePow; break;
    default: ret = false; cout << index.topic; break;
    }

    if(ptr) index.value = ptr[index.id];
    return ret;
}

void Alarm_Object::clearEle(sDataItem &index)
{
    int start=0, end=0; if(index.id) {
        sDevCfg *cfg = &cm::masterDev()->cfg;
        if(DType::Line == index.type) {      /*type为相*/
            int id = index.id; if(id) id -= 1;
            int size = cfg->nums.lineNum;   /*相数量*/
            int num = cfg->nums.outputNum / size;   /*输出位数量*/
            start = id * num; end = (id+1) * num;
            if(cfg->param.devSpec == 1) {   /*A系列*/
                num = cfg->nums.loopNum / size;
                start = id * num; end = (id+1) * num;
                OP_Core::bulid()->clearEle_A(start, end);   /*清除A系列电能*/
            } else OP_Core::bulid()->clearEle(start, end);  /*清除BCD系列电能*/
            for(int i=start; i<end; ++i) cm::masterDev()->line.ele[i] = 0;
        } else if((DType::Loop == index.type) && (cfg->param.devSpec > 1)) {    /*type为回路并且不为A系列*/
            for(int i=0; i<index.id; ++i) {
                if(i) start += cfg->nums.loopEachNum[i-1];
                end += cfg->nums.loopEachNum[i];
            } OP_Core::bulid()->clearEle(start, end);
            for(int i=start; i<end; ++i) cm::masterDev()->loop.ele[i] = 0;
        } else {            
            if(1 == cfg->param.devSpec) {
                OP_Core::bulid()->clearEle_A(index.id);
            } else {
                OP_Core::bulid()->clearEle(index.id);
            } cm::masterDev()->output.ele[index.id-1] = 0;
        }
    } else {
        OP_Core::bulid()->clearEle(0);  /*id为0则清除所有电能*/
    }
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
        if(index.rw){   /*写*/
            if(index.id) ptr[index.id-1] = index.value; /*单个设置*/
            else setAll(ptr, index.value, obj->size);   /*统一设置*/
            if(DTopic::Ele == index.topic) clearEle(index);
        } else index.value = ptr[index.id];     /*读*/
    }

    return ret;
}

bool Alarm_Object::tgValue(sDataItem &index)
{
    bool ret = true;
    if(index.topic > DTopic::Pow) {
        sTgObjData *tg = &(cm::devData(index.addr)->tg);
        if(DType::CabTg == index.type) tg = &(cm::devData(index.addr)->cabTg);
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
/**
 * 更新元数据
 */
bool Alarm_Object::upMetaData(sDataItem &index)
{
    bool  ret = false; uchar addr = index.addr;
    if(addr >= 0xff) index.addr = 0;

    if(index.addr > DEV_NUM) {cout << index.addr; return ret;}  /*addr>20，返回false*/
    switch (index.type) {
    case DType::Sensor: return sensorValue(index);
    case DType::Tg: case DType::CabTg: return tgValue(index);
    }

    switch (index.topic) {
    case DTopic::HdaEle: case DTopic::Ele: ret = eleValue(index); break;
    case DTopic::Relay: ret = relayUnitValue(index); break;
    case DTopic::ArtPow: case DTopic::ReactivePow: case DTopic::LineVol:
    case DTopic::PF: ret = powPfValue(index); break;
    default: ret = alarmUnitValue(index); break;
    } index.addr = addr;

    return ret;
}
