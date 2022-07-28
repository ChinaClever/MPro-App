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

    size = ptr->groupSize; in << size;
    for(int i=0; i<size; ++i) in << toByteArray(ptr->group[i]);

    size = ptr->outputSize; in << size;
    for(int i=0; i<size; ++i) in << toByteArray(ptr->output[i]);

    size = ptr->envSize; in << size;
    for(int i=0; i<size; ++i) in << toByteArray(ptr->env[i]);

    in << toByteArray(ptr->tg);
    in << toByteArray(ptr->rtu);
    in << toByteArray(ptr->cfg);
    //in << toByteArray(ptr->uut);
    in << ptr->lps;

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

    out >> size; ptr->outputSize = size;
    for(int i=0; i<size; ++i) {out >> v; ptr->output[i] = toStruct<c_sObjData>(v);}

    out >> size; ptr->envSize = size;
    for(int i=0; i<size; ++i) {out >> v; ptr->env[i] = toStruct<c_sEnvData>(v);}

    out >> v; ptr->tg = toStruct<sTgObjData>(v);
    out >> v; ptr->rtu = toStruct<sRtuBoard>(v);
    out >> v; ptr->cfg = toStruct<sDevCfg>(v);
    //out >> v; ptr->uut = toStruct<sUutInfo>(v);
    out >> ptr->lps;

    return out;
}
