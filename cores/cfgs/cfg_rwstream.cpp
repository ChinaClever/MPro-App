/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cfg_rwstream.h"

Cfg_RwStream::Cfg_RwStream(set::_sDevData *data) : mDevData{data}
{

}


QDataStream& operator<<(QDataStream& in, Cfg_RwStream& data)
{
    using namespace cm;
    set::_sDevData *ptr = data.mDevData;

    in << ptr->version;
    uchar size = ptr->lineSize = LINE_NUM; in << size;
    for(int i=0; i<size; ++i) in << toByteArray(ptr->line[i]);

    size = ptr->loopSize = LOOP_NUM; in << size;
    for(int i=0; i<size; ++i) in << toByteArray(ptr->loop[i]);

    size = ptr->outputSize = OUTPUT_NUM; in << size;
    for(int i=0; i<size; ++i) in << toByteArray(ptr->output[i]);

    size = ptr->envSize = SENOR_NUM; in << size;
    for(int i=0; i<size; ++i) in << toByteArray(ptr->env[i]);
    in << toByteArray(ptr->tg);
    in << toByteArray(ptr->info);
    in << toByteArray(ptr->login);

    return in;
}

QDataStream& operator>>(QDataStream& out, Cfg_RwStream& data)
{
    using namespace cm; using namespace set;
    set::_sDevData *ptr = data.mDevData;
    QByteArray v; uchar size;

    out >> ptr->version;
    out >> size; ptr->lineSize = size;
    for(int i=0; i<size; ++i) {out >> v; ptr->line[i] = toStruct<_sObjData>(v);}

    out >> size; ptr->loopSize = size;
    for(int i=0; i<size; ++i) {out >> v; ptr->loop[i] = toStruct<_sObjData>(v);}

    out >> size; ptr->outputSize = size;
    for(int i=0; i<size; ++i) {out >> v; ptr->output[i] = toStruct<_sObjData>(v);}

    out >> size; ptr->envSize = size;
    for(int i=0; i<size; ++i) {out >> v; ptr->env[i] = toStruct<_sEnvData>(v);}
    out >> v; ptr->tg = toStruct<sTgObjData>(v);
    out >> v; ptr->info = toStruct<sDevInfo>(v);
    out >> v; ptr->login = toStruct<sDevLogin>(v);

    return out;
}
