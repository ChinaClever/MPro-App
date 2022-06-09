/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_rwinitial.h"

Set_RwInitial::Set_RwInitial(QObject *parent)
    : Set_RwUnserialize{parent}
{

}

bool Set_RwInitial::initialData()
{
    sDevData *dev = cm::masterDev();
    initDevInfo(dev->info);
    initDevData(dev);
    return true;
}

void Set_RwInitial::initDevInfo(sDevInfo &it)
{
    it.opNum = 3;
    it.lineNum = LINE_NUM;
    it.loopNum = LOOP_NUM / 2;
    it.outputNum = OUTPUT_NUM / 2;

    for(int i=0; i<it.loopNum; ++i) {
        it.loopEnds[i] = 8*(i+1);
        it.loopStarts[i] = 8*i;
    }
}

void Set_RwInitial::setVolAlarm(sAlarmUnit &it, uchar size, uint rated)
{
    for(int i=0; i<size; ++i) {
        it.en[i] = 1;
        it.rated[i] = rated;
        it.max[i] = it.crMax[i] = rated * 1.2;
        it.min[i] = it.crMin[i] = rated * 0.8;
    }
}

void Set_RwInitial::setAlarmUnit(sAlarmUnit &it, uchar size, uint rated)
{
    for(int i=0; i<size; ++i) {
        it.en[i] = 1;
        it.rated[i] = rated;
        it.max[i] = rated;
        it.crMax[i] = rated * 0.8;
    }
}

void Set_RwInitial::initObjData(sObjData &it, uchar size, uint curRated)
{
    setAlarmUnit(it.cur, size, curRated);
    setVolAlarm(it.vol, size, 220*COM_RATE_VOL);
    setAlarmUnit(it.pow, size, 220*curRated/COM_RATE_CUR/COM_RATE_VOL);
}

void Set_RwInitial::setTgVol(sTgUnit &it, uint rated)
{
    it.rated = rated;
    it.max = it.crMax = rated * 1.2;
    it.min = it.crMin = rated * 0.8;
}

void Set_RwInitial::setTgUnit(sTgUnit &it, uint rated)
{
    it.rated = rated;
    it.max = rated;
    it.crMax = rated * 0.8;
}

void Set_RwInitial::initTgObjData(sTgObjData &it)
{
    setTgVol(it.vol, 220*COM_RATE_VOL);
    setTgUnit(it.cur, 32*COM_RATE_CUR);
    setTgUnit(it.pow, 220*32);
}

void Set_RwInitial::initEnvData(sEnvData &it)
{
    uchar size = SENOR_NUM;
    setAlarmUnit(it.tem, size, 60);
    setAlarmUnit(it.hum, size, 99);
}

void Set_RwInitial::initDevData(sDevData *dev)
{
    initObjData(dev->line, LINE_NUM, 32*COM_RATE_CUR);
    initObjData(dev->loop, LOOP_NUM, 16*COM_RATE_CUR);
    initObjData(dev->output, OUTPUT_NUM, 10*COM_RATE_CUR);
    initRelayUnit(dev->output.relay, OUTPUT_NUM);
    initTgObjData(dev->tg);
    initEnvData(dev->env);
}

void Set_RwInitial::initRelayUnit(sRelayUnit &it, uchar size)
{
    for(int i=0; i<size; ++i) it.en[i] = 1;
}
