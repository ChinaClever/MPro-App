/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "data_object.h"

Data_Object::Data_Object()
{
    mDev = cm::masterDev();
}

uint Data_Object::summation(const uint *ptr, const QList<int> &ls)
{
     uint ret = 0;
     foreach(auto i, ls) ret += ptr[i];
     return ret;
}

uint Data_Object::summation(const uint *ptr, int start, int end)
{
    QList<int> ls;
    for(int i=start; i<end; i++) ls << i;
    return summation(ptr, ls);
}

void Data_Object::sumAlarmUnit(int id, sAlarmUnit &dest, const sAlarmUnit &src, int start, int end)
{
    dest.value[id] = summation(src.value, start, end);
}

void Data_Object::sumAlarmUnit(int id, sAlarmUnit &dest, const sAlarmUnit &src, const QList<int> &ls)
{
    dest.value[id] = summation(src.value, ls);
}

uint Data_Object::averageValue(const uint *ptr, const QList<int> &ls)
{
     QList<uint> list;  uint ret = 0;
     foreach(auto i, ls) if(ptr[i]) list << ptr[i];
     if(list.size()) {
         std::sort(list.begin(), list.end());
         int k = (list.size() + 1) / 2;
         if(k < list.size()) ret = list.at(k);
         else ret = list.first();
     }

     return ret;
}
uint Data_Object::averageValue(const uint *ptr, int start, int end)
{   
    QList<int> ls;
    for(int i=start; i<end; ++i) ls << i;
    return averageValue(ptr, ls);
}

void Data_Object::averAlarmUnit(int id, sAlarmUnit &dest, const sAlarmUnit &src, int start, int end)
{
    dest.value[id] = averageValue(src.value, start, end);
}

void Data_Object::averAlarmUnit(int id, sAlarmUnit &dest, const sAlarmUnit &src, const QList<int> &ls)
{
    dest.value[id] = averageValue(src.value, ls);
}

uint Data_Object::calPf(int id, sObjData &obj)
{
    uint ret = 0;
    uint pow = obj.pow.value[id];
    uint art = obj.artPow[id];
    if(pow && art) {
        ret = (pow * 100.0) / art;
        if(ret > 99) ret = 99;
    }

    return ret;
}

void Data_Object::sumObjData(int id, sObjData &dest, const sObjData &src, int start, int end)
{
    QList<int> ls;
    for(int i=start; i<end; ++i) ls << i;
    return sumObjData(id, dest, src, ls);
}

void Data_Object::sumObjData(int id, sObjData &dest, const sObjData &src, const QList<int> &ls)
{
    averAlarmUnit(id, dest.vol, src.vol, ls);
    sumAlarmUnit(id, dest.cur, src.cur, ls);
    sumAlarmUnit(id, dest.pow, src.pow, ls);

    dest.ele[id] = summation(src.ele, ls);
    dest.artPow[id] = summation(src.artPow, ls);
    dest.reactivePow[id] = summation(src.reactivePow, ls);
    dest.pf[id] = calPf(id, dest);
}

void Data_Object::loopData(int id, int start, int end)
{
    sumObjData(id, mDev->loop, mDev->output, start, end);
    uint *ptr = mDev->output.vol.value; uint sw = 0;
    if(ptr[start] || ptr[end-1]) sw = 1;
    mDev->loop.relay.sw[id] = sw;
}

void Data_Object::lineData(int id, int start, int end)
{
    sumObjData(id, mDev->line, mDev->loop, start, end);
}

