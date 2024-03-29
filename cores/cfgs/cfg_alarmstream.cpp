/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cfg_alarmstream.h"

Cfg_AlarmStream::Cfg_AlarmStream(cfg::_sDevData *data) : mDevData{data}
{

}


QDataStream& operator<<(QDataStream& in, Cfg_AlarmStream& data)
{
    using namespace cm;
    cfg::_sDevData *ptr = data.mDevData;

    in << ptr->version;
    uchar size = ptr->lineSize = LINE_NUM; in << size;
    for(int i=0; i<size; ++i) in << toByteArray(ptr->line[i]);

    size = ptr->loopSize = LOOP_NUM; in << size;
    for(int i=0; i<size; ++i) in << toByteArray(ptr->loop[i]);

    size = ptr->groupSize = GROUP_NUM; in << size;
    for(int i=0; i<size; ++i) in << toByteArray(ptr->group[i]);

    size = ptr->dualSize = OUTPUT_NUM; in << size;
    for(int i=0; i<size; ++i) in << toByteArray(ptr->dual[i]);

    size = ptr->cabLineSize = LINE_NUM; in << size;
    for(int i=0; i<size; ++i) in << toByteArray(ptr->cabLine[i]);

    size = ptr->cabLoopSize = LOOP_NUM; in << size;
    for(int i=0; i<size; ++i) in << toByteArray(ptr->cabLoop[i]);

    size = ptr->outputSize = OUTPUT_NUM; in << size;
    for(int i=0; i<size; ++i) in << toByteArray(ptr->output[i]);

    size = ptr->envSize = SENOR_NUM; in << size;
    for(int i=0; i<size; ++i) in << toByteArray(ptr->env[i]);
    in << toByteArray(ptr->tg); in << toByteArray(ptr->cabTg);

    for(int i=0; i<6; ++i) {size = 0; in << size; in << QByteArray();}
    for(int i=0; i<6; ++i) in << QByteArray();

    return in;
}

QDataStream& operator>>(QDataStream& out, Cfg_AlarmStream& data)
{
    using namespace cm; using namespace cfg;
    cfg::_sDevData *ptr = data.mDevData;
    QByteArray v; uchar size;

    out >> ptr->version;
    out >> size; ptr->lineSize = size;
    for(int i=0; i<size; ++i) {out >> v; ptr->line[i] = toStruct<_sObjData>(v);}

    out >> size; ptr->loopSize = size;
    for(int i=0; i<size; ++i) {out >> v; ptr->loop[i] = toStruct<_sObjData>(v);}

    out >> size; ptr->groupSize = size;
    for(int i=0; i<size; ++i) {out >> v; ptr->group[i] = toStruct<_sObjData>(v);}

    out >> size; ptr->dualSize = size;
    for(int i=0; i<size; ++i) {out >> v; ptr->dual[i] = toStruct<_sObjData>(v);}

    out >> size; ptr->cabLineSize = size;
    for(int i=0; i<size; ++i) {out >> v; ptr->cabLine[i] = toStruct<_sObjData>(v);}

    out >> size; ptr->cabLoopSize = size;
    for(int i=0; i<size; ++i) {out >> v; ptr->cabLoop[i] = toStruct<_sObjData>(v);}

    out >> size; ptr->outputSize = size;
    for(int i=0; i<size; ++i) {out >> v; ptr->output[i] = toStruct<_sObjData>(v);}

    out >> size; ptr->envSize = size;
    for(int i=0; i<size; ++i) {out >> v; ptr->env[i] = toStruct<_sEnvData>(v);}
    out >> v; ptr->tg = toStruct<sTgObjData>(v);
    out >> v; ptr->cabTg = toStruct<sTgObjData>(v);

    for(int i=0; i<6; ++i) {out >> size; out >> v;}
    for(int i=0; i<6; ++i) out >> v;

    return out;
}
