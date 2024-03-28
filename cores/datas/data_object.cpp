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


uint Data_Object::getMetaValue(const QList<uint>& array)
{
    uint length = array.size();
    uint middleIndex = length / 2;

    QMap<int, uint> countMap;

    for (uint value : array) {
        if (value != 0) {
            countMap[value]++;
        }
    }

    uint maxCount = 0;
    uint maxValue = 0;

    for (auto it = countMap.begin(); it != countMap.end(); ++it) {
        uint value = it.key();
        uint count = it.value();

        if (count > maxCount) {
            maxCount = count;
            maxValue = value;
        }
    }

    if (maxCount > 1) {
        return maxValue;  // 返回非零值中出现次数最多的值
    } else {
        QList<uint> sortedArray = array;
        std::sort(sortedArray.begin(), sortedArray.end());
        return sortedArray[middleIndex];  // 返回排序后的中间值
    }
}

uint Data_Object::averageValue(const uint *ptr, const QList<int> &ls)
{
    QList<uint> list; // uint ret = 0; QList<uint> tmp;
    // foreach(auto i, ls) if(ptr[i]) if(!list.contains(ptr[i])) list << ptr[i];
    // if(list.size()) {
    //     std::sort(list.begin(), list.end());
    //     int k = (list.size() + 1) / 2;
    //     if(k < list.size()) ret = list.at(k);
    //     else ret = list.last();
    // }

    foreach(auto i, ls) list << ptr[i];
    uint ret = getMetaValue(list);
    if(ret < 50*COM_RATE_VOL) ret = 0;

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
        ret = (pow * COM_RATE_PF) / art;
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
    dest.apparentEle[id] = summation(src.apparentEle, ls);
    dest.reactivePow[id] = summation(src.reactivePow, ls);
    dest.reactiveEle[id] = summation(src.reactiveEle, ls);
    dest.pf[id] = calPf(id, dest);
}

void Data_Object::loopData(int id, int start, int end)
{
    sumObjData(id, mDev->loop, mDev->output, start, end);   /*计算电压、电流、功率、无功功率、视在功率、电能、功率因数*/
    uint *ptr = mDev->output.vol.value; uint sw = 0;    /*获取输出位电压的值*/
    if(mDev->loop.cur.value[id] > 0.2*COM_RATE_CUR) sw = 1; /*回电流>20A将SW置1*/
    for(int i=start; i<end; ++i) if(ptr[i] > 50 *COM_RATE_VOL) sw = 1;  /*输出位电压的值>500V将SW置1*/
    if(!mDev->cfg.param.isBreaker) {sw = 2;} mDev->loop.relay.sw[id] = sw;  /*如果没有断路器，回路开关状态置为复位*/
}

void Data_Object::loopBreaker(int id)
{
    uint sw = 0; uint *cnt = &mDev->loop.relay.cnt[id];// mDev->cfg.nums.loopEachNum[id] = 0;   /*获取继电器的控制次数*/
    if(mDev->loop.cur.value[id] > 0.2 * COM_RATE_CUR) sw = 1;   /*回电流>20A将SW置1*/
    if(mDev->loop.vol.value[id] > 50 *COM_RATE_VOL) sw = 1;     /*回路电压的值>500V将SW置1*/
    if(mDev->loop.pow.value[id] > 50) sw = 1;       /*回路有功功率的值>50将SW置1*/
    if(sw) *cnt = 5; else if(*cnt) if(--(*cnt)) sw = 1;
    if(!mDev->cfg.param.isBreaker) {sw = 2;}    /*如果没有断路器，回路开关状态置为复位*/
    mDev->loop.relay.sw[id] = sw;
}

void Data_Object::lineData(int id, int start, int end)
{
    int dtc = mDev->dtc.fault;
    static uint volArray[LINE_NUM] = {0, 0, 0};
    sumObjData(id, mDev->line, mDev->loop, start, end);
    uint vol = mDev->line.vol.value[id];
    if(vol > 196*COM_RATE_VOL) {
        volArray[id] = mDev->line.vol.value[id];
    } else if(vol < COM_MIN_VOL && dtc == FaultCode::DTC_OK) {
       mDev->line.vol.value[id] = volArray[id];
    }
}

void Data_Object::lineDataNoLoop(int id, int start, int end)
{
    sumObjData(id, mDev->line, mDev->output, start, end);
}
