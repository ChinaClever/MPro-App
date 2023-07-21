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

void Cfg_AlarmInitial::setVolAlarm(sAlarmUnit &it, uchar size, uint rated)
{
    for(int i=0; i<size; ++i) {
        it.en[i] = 1; it.rated[i] = rated;
        it.max[i] = it.crMax[i] = rated * 1.2;
        it.min[i] = it.crMin[i] = rated * 0.8;
    }
}

void Cfg_AlarmInitial::setAlarmUnit(sAlarmUnit &it, uchar size, uint rated)
{
    for(int i=0; i<size; ++i) {
        it.en[i] = 1;
        it.max[i] = rated;
        it.rated[i] = rated;
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
    setTgVol(it.vol, 600*COM_RATE_VOL);
    setTgUnit(it.cur, 80*COM_RATE_CUR);
    setTgUnit(it.pow, 600*80);
}

void Cfg_AlarmInitial::initEnvData(sEnvData &it)
{
    uchar size = SENOR_NUM;
    setAlarmUnit(it.tem, size, 100*COM_RATE_TEM);
    setAlarmUnit(it.hum, size, 99*COM_RATE_HUM);

    for(int i=0; i<size; ++i) {
        it.tem.min[i] = 40*COM_RATE_TEM;
        it.tem.crMin[i] = 40*COM_RATE_TEM;
        it.tem.crMax[i] = 100*COM_RATE_TEM;
        it.hum.crMax[i] = 99*COM_RATE_HUM;

        it.tem.rated[i] = 150*COM_RATE_TEM;
        it.hum.rated[i] = 100*COM_RATE_HUM;
    }
}

void Cfg_AlarmInitial::initDevData(sDevData *dev)
{
    initObjHda(dev->line, LINE_NUM);
    initObjData(dev->line, LINE_NUM, 32);
    initObjData(dev->loop, LOOP_NUM, 16);
    initObjData(dev->cabLine, LINE_NUM, 63);
    initObjData(dev->cabLoop, LOOP_NUM, 32);
    initObjData(dev->dual, OUTPUT_NUM, 20);
    initObjData(dev->group, GROUP_NUM, 64);
    initObjData(dev->output, OUTPUT_NUM, 10);
    //initRelayUnit(dev->dual.relay, OUTPUT_NUM);
    //initRelayUnit(dev->group.relay, GROUP_NUM);
    initRelayUnit(dev->output.relay, OUTPUT_NUM);
    initTgObjData(dev->cabTg);
    initTgObjData(dev->tg);
    initEnvData(dev->env);
}

void Cfg_AlarmInitial::initRelayUnit(sRelayUnit &it, uchar size)
{
    for(int i=0; i<size; ++i) {
        it.disabled[i] = 0;
        it.lifeEn[i] = 0;
        it.maxCnt[i] = 50000;
        it.resetDelay[i] = 5;
    }
}

void Cfg_AlarmInitial::initObjHda(sObjData &it, uchar size)
{
    for(int i=0; i<size; ++i) {
        it.hdaEle[i] = 1;
        it.vol.hda[i] = 1;
        it.pow.hda[i] = 1;
    }
}
