/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cfg_alarminitial.h"

Cfg_AlarmInitial::Cfg_AlarmInitial()
{

}

bool Cfg_AlarmInitial::initialData()
{
    sDevData *dev = cm::masterDev();    
    initDevData(dev);
    return true;
}

bool Cfg_AlarmInitial::initialParam()
{
    sDevData *dev = cm::masterDev();
    dev->cfg.param.isBreaker = 1;
    initDevInfo(dev->cfg.nums);
    return true;
}

void Cfg_AlarmInitial::initDevInfo(sDevNums &it)
{
    it.boardNum = 3;
    it.lineNum = LINE_NUM;
    it.loopNum = LOOP_NUM / 2;
    it.outputNum = OUTPUT_NUM / 2;
    for(uint i=0; i<it.loopNum; ++i) {
        it.loopEnds[i] = 8*(i+1);
        it.loopStarts[i] = 8*i;
    }
}

void Cfg_AlarmInitial::setVolAlarm(sAlarmUnit &it, uchar size, uint rated)
{
    for(int i=0; i<size; ++i) {
        it.en[i] = 1;
        it.rated[i] = rated;
        it.max[i] = it.crMax[i] = rated * 1.2;
        it.min[i] = it.crMin[i] = rated * 0.8;
    }
}

void Cfg_AlarmInitial::setAlarmUnit(sAlarmUnit &it, uchar size, uint rated)
{
    for(int i=0; i<size; ++i) {
        it.en[i] = 1;
        it.rated[i] = rated;
        it.max[i] = rated;
        it.crMax[i] = rated * 0.8;
    }
}

void Cfg_AlarmInitial::initObjData(sObjData &it, uchar size, uint curRated)
{
    setAlarmUnit(it.pow, size, 220*curRated);
    setVolAlarm(it.vol, size, 220*COM_RATE_VOL);
    setAlarmUnit(it.cur, size, curRated*COM_RATE_CUR);
}

void Cfg_AlarmInitial::setTgVol(sTgUnit &it, uint rated)
{
    it.rated = rated;
    it.max = it.crMax = rated * 1.2;
    it.min = it.crMin = rated * 0.8;
}

void Cfg_AlarmInitial::setTgUnit(sTgUnit &it, uint rated)
{
    it.max = rated;
    it.rated = rated;
    it.crMax = rated * 0.8;
}

void Cfg_AlarmInitial::initTgObjData(sTgObjData &it)
{
    setTgVol(it.vol, 220*COM_RATE_VOL);
    setTgUnit(it.cur, 32*COM_RATE_CUR);
    setTgUnit(it.pow, 220*32);
}

void Cfg_AlarmInitial::initEnvData(sEnvData &it)
{
    uchar size = SENOR_NUM;
    setAlarmUnit(it.tem, size, 60*COM_RATE_TEM);
    setAlarmUnit(it.hum, size, 99*COM_RATE_HUM);
}

void Cfg_AlarmInitial::initDevData(sDevData *dev)
{
    initObjData(dev->line, LINE_NUM, 32);
    initObjData(dev->loop, LOOP_NUM, 16);
    initObjData(dev->dual, OUTPUT_NUM, 20);
    initObjData(dev->group, GROUP_NUM, 14);
    initObjData(dev->output, OUTPUT_NUM, 10);
    initRelayUnit(dev->dual.relay, OUTPUT_NUM);
    initRelayUnit(dev->group.relay, GROUP_NUM);
    initRelayUnit(dev->output.relay, OUTPUT_NUM);
    initTgObjData(dev->tg);
    initEnvData(dev->env);
}

void Cfg_AlarmInitial::initRelayUnit(sRelayUnit &it, uchar size)
{
    for(int i=0; i<size; ++i) {
        it.en[i] = 1;
        it.resetDelay[i] = 5;
    }
}
