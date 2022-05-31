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

uint Data_Object::summation(const uint *ptr, int start, int end)
{
    uint ret = 0;
    for(int i=start; i<end; i++) {
        ret += ptr[i];
    }
    return ret;
}

void Data_Object::sumAlarmUnit(int id, sAlarmUnit &dest, const sAlarmUnit &src, int start, int end)
{
    dest.value[id] = summation(src.value, start, end);
}

uint Data_Object::averageValue(const uint *ptr, int start, int end)
{
    QList<uint> list;
    for(int i=start; i<end; ++i) list << ptr[i];
    std::sort(list.begin(), list.end());
    int k = (list.size() + 1) / 2;
    return list.at(k);
}

void Data_Object::averAlarmUnit(int id, sAlarmUnit &dest, const sAlarmUnit &src, int start, int end)
{
    dest.value[id] = averageValue(src.value, start, end);
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
    averAlarmUnit(id, dest.vol, src.vol, start, end);
    sumAlarmUnit(id, dest.cur, src.cur, start, end);
    sumAlarmUnit(id, dest.pow, src.pow, start, end);

    dest.ele[id] = summation(src.ele, start, end);
    dest.artPow[id] = summation(src.artPow, start, end);
    dest.reactivePow[id] = summation(src.reactivePow, start, end);
    dest.pf[id] = calPf(id, dest);
}

void Data_Object::loopData(int id, int start, int end)
{
    sumObjData(id, mDev->loop, mDev->output, start, end);
}

void Data_Object::lineData(int id, int start, int end)
{
    sumObjData(id, mDev->line, mDev->loop, start, end);
}

