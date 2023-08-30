/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ftp_data.h"

Ftp_Data::Ftp_Data()
{

}

void Ftp_Data::upDevData()
{
    int num = cm::masterDev()->cfg.nums.slaveNum;
    for(int i=0; i<=num; ++i) {
        if(cm::devData(i)->offLine > 1) {
            upOutletData(i);
            upLineData(i);
            upLoopData(i);
            upDualData(i);
            upTotalData(i);
        }
    }
}

void Ftp_Data::upRelayUnit(int id, const sRelayUnit &relay, QStringList &values)
{
    if(relay.size) {
        if(cm::cn()) {
            if(relay.sw[id])  values << "接通"; else values << "断开";
        } else {
            if(relay.sw[id])  values << "on"; else values << "off";
        }
    }
}

void Ftp_Data::upObjData(int id, const QString &name, const sObjData *obj, QStringList &values)
{
    values << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    values << name; upRelayUnit(id, obj->relay, values);

    const sAlarmUnit *unit = &obj->vol;
    if(unit->size) values << QString::number(unit->value[id] / COM_RATE_VOL);

    unit = &obj->cur;
    if(unit->size) values << QString::number(unit->value[id] / COM_RATE_CUR);

    unit = &obj->pow;
    if(unit->size) values << QString::number(unit->value[id] / COM_RATE_POW);

    if(obj->size) {
        values << QString::number(obj->artPow[id] / COM_RATE_POW);
        values << QString::number(obj->pf[id] / COM_RATE_PF);
        values << QString::number(obj->ele[id] / COM_RATE_ELE);
    }
}

void Ftp_Data::upLineData(int addr)
{
    if(!ftpCfg.line) return;
    sObjData *obj = &cm::devData(addr)->line;
    for (int i = 0; i < obj->size; ++i) {
        QString name = "L" + QString::number(i+1);
        QStringList values; upObjData(i, name, obj, values);
        if(values.size() > 2) csv_appendLine(addr, values);
    }
}


void Ftp_Data::upLoopData(int addr)
{
    if(!ftpCfg.loop) return;
    sObjData *obj = &cm::devData(addr)->loop;
    for (int i = 0; i < obj->size; ++i) {
        QString name = "C" + QString::number(i+1);
        QStringList values; upObjData(i, name, obj, values);
        if(values.size() > 2) csv_appendLoop(addr, values);
    }
}


void Ftp_Data::upOutletData(int addr)
{
    if(!ftpCfg.outlet) return;
    sObjData *obj = &cm::devData(addr)->output;
    for (int i = 0; i < obj->size; ++i) {
        QStringList values;
        QString name = obj->name[i];
        upObjData(i, name, obj, values);
        if(values.size() > 2) csv_appendOutlet(addr, values);
    }
}


void Ftp_Data::upDualData(int addr)
{
    if(!ftpCfg.dualPower) return;
    sObjData *obj = &cm::devData(addr)->dual;
    for (int i = 0; i < obj->size; ++i) {
        QStringList values;
        QString name = obj->name[i];
        upObjData(i, name, obj, values);
        if(values.size() > 2) csv_appendDual(addr, values);
    }
}

void Ftp_Data::upTotalData(int addr)
{
    if(!ftpCfg.total) return;
    QStringList values;
    sTgObjData *obj = &cm::devData(addr)->tg;
    values << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    values << QString::number(obj->pow.value / COM_RATE_POW);
    values << QString::number(obj->artPow / COM_RATE_POW);
    values << QString::number(obj->pf / COM_RATE_PF);
    values << QString::number(obj->ele / COM_RATE_ELE);
    csv_appendTotal(addr, values);
}
