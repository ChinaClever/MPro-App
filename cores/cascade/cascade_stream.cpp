/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cascade_stream.h"

Cascade_Stream::Cascade_Stream(c_sDevData *data) : mDevData{data}
{

}

Cascade_Stream &Cascade_Stream::operator=(const Cascade_Stream &other)
{
    mDevData = other.mDevData;
    return *this;
}

QDataStream& operator<<(QDataStream& in, Cascade_Stream& data)
{
    using namespace cm;
    c_sDevData *ptr = data.mDevData;
    in << ptr->version << ptr->id << ptr->hz;

    uchar size = ptr->lineSize; in << size;
    for(int i=0; i<size; ++i) in << toByteArray(ptr->line[i]);

    size = ptr->loopSize; in << size;
    for(int i=0; i<size; ++i) in << toByteArray(ptr->loop[i]);

    size = ptr->groupSize; if(!size) size = ptr->group->relay.size;
    in << size; for(int i=0; i<size; ++i) in << toByteArray(ptr->group[i]);

    size = ptr->dualSize; in << size;
    for(int i=0; i<size; ++i) in << toByteArray(ptr->dual[i]);

    size = ptr->cabLineSize; in << size;
    for(int i=0; i<size; ++i) in << toByteArray(ptr->cabLine[i]);

    size = ptr->cabLoopSize; in << size;
    for(int i=0; i<size; ++i) in << toByteArray(ptr->cabLoop[i]);

    size = ptr->outputSize; if(!size) size = ptr->output->relay.size;
    in << size; for(int i=0; i<size; ++i) in << toByteArray(ptr->output[i]);

    size = ptr->envSize; if(!size) size = SENOR_NUM;
    in << size; for(int i=0; i<size; ++i) in << toByteArray(ptr->env[i]);

    in << toByteArray(ptr->tg);
    in << toByteArray(ptr->cabTg);
    in << toByteArray(ptr->rtu);
    in << toByteArray(ptr->dtc);
    in << toByteArray(ptr->cfg);
    in << toByteArray(ptr->proc);
    in << ptr->lps << ptr->dc;

    for(int i=0; i<6; ++i) in << ptr->reserve[i];
    for(int i=0; i<6; ++i) {size = 0; in << size; in << QByteArray();}
    for(int i=0; i<6; ++i) in << QByteArray();

    return in;
}

QDataStream& operator>>(QDataStream& out, Cascade_Stream& data)
{
    using namespace cm;
    c_sDevData *ptr = data.mDevData;
    out >> ptr->version >> ptr->id >> ptr->hz; QByteArray v; uchar size;

    out >> size; ptr->lineSize = size;
    for(int i=0; i<size; ++i) {out >> v; ptr->line[i] = toStruct<c_sObjData>(v);}

    out >> size; ptr->loopSize = size;
    for(int i=0; i<size; ++i) {out >> v; ptr->loop[i] = toStruct<c_sObjData>(v);}

    out >> size; ptr->groupSize = size;
    for(int i=0; i<size; ++i) {out >> v; ptr->group[i] = toStruct<c_sObjData>(v);}

    out >> size; ptr->dualSize = size;
    for(int i=0; i<size; ++i) {out >> v; ptr->dual[i] = toStruct<c_sObjData>(v);}

    out >> size; ptr->cabLineSize = size;
    for(int i=0; i<size; ++i) {out >> v; ptr->cabLine[i] = toStruct<c_sObjData>(v);}

    out >> size; ptr->cabLoopSize = size;
    for(int i=0; i<size; ++i) {out >> v; ptr->cabLoop[i] = toStruct<c_sObjData>(v);}

    out >> size; ptr->outputSize = size;
    for(int i=0; i<size; ++i) {out >> v; ptr->output[i] = toStruct<c_sObjData>(v);}

    out >> size; ptr->envSize = size; if(!size) size = SENOR_NUM;
    for(int i=0; i<size; ++i) {out >> v; ptr->env[i] = toStruct<c_sEnvData>(v);}

    out >> v; ptr->tg = toStruct<sTgObjData>(v);
    out >> v; ptr->cabTg = toStruct<sTgObjData>(v);
    out >> v; ptr->rtu = toStruct<sRtuBoard>(v);
    out >> v; ptr->dtc = toStruct<sFaultCode>(v);
    out >> v; ptr->cfg = toStruct<sDevCfg>(v);
    out >> v; ptr->proc = toStruct<sProcState>(v);
    out >> ptr->lps >> ptr->dc;

    for(int i=0; i<6; ++i) out >> ptr->reserve[i];
    for(int i=0; i<6; ++i) {out >> size; out >> v;}
    for(int i=0; i<6; ++i) out >> v;

    return out;
}
