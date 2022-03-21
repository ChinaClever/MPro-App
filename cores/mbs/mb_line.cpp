#include "mb_line.h"

Mb_Line::Mb_Line(QObject *parent) : Mb_Object{parent}
{

}

void Mb_Line::mbLineUpdate()
{
    upTgData();
    upLineData();
    upLineThreshold();
}

void Mb_Line::upTgData()
{
    vshort vs; upDevInfo();
    sTgObjData *tg = &(mDevData->tg);
    vs << tg->vol.value << tg->cur.value << tg->pow.value;
    vs << tg->artPow - tg->pow.value << tg->pf;
    vs << tg->ele / 65536 << tg->ele % 65536;
    vs << mDevData->hz;
    setRegs(MbReg_Totals, vs);
}

void Mb_Line::upLineData()
{
    vshort vs; int size = LINE_NUM;
    sObjData *line = &(mDevData->line);
    appendData(size, line->vol.value, vs);
    appendData(size, line->cur.value, vs);
    appendData(size, line->pow.value, vs);
    appendData(size, line->reactivePow, vs);
    appendData(size, line->pf, vs);
    setRegs(MbReg_Lines, vs);    
}

void Mb_Line::upLineThreshold()
{
    sObjData *obj = &(mDevData->line); vshort vs;
    vs << mDevData->tg.cur.max << mDevData->tg.cur.min;

    appendAlarm(obj->cur, vs);
    setRegs(MbReg_SetLine, vs);
    vs.clear();

    vs << mDevData->tg.vol.max << mDevData->tg.vol.min;
    appendAlarm(obj->vol, vs);

    vs << mDevData->tg.cur.crMax;
    appendData(obj->size, obj->cur.crMax, vs);
    setRegs(1141, vs);
}
