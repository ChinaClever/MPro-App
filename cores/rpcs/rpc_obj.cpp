/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "rpc_obj.h"

Rpc_Obj::Rpc_Obj(QObject *parent)
    : QObject{parent}
{

}

int Rpc_Obj::pduMetaData(uchar addr,  uchar type, uchar topic, uchar sub, uchar id)
{
    //jcon::JsonRpcServer::clientEndpoint()->peerAddress().toString();
    sDataItem *it = &mIt; it->addr = addr; it->type = type; if(id) id--;
    it->topic = topic; it->subtopic = sub; it->id = id;
    it->rw = 0; it->value = 0;
    Set_Core::bulid()->upIndexValue(mIt);
    return mIt.value;
}

bool Rpc_Obj::pduSetData(uchar addr,  uchar type, uchar topic, uchar sub, uchar id, uint value)
{
    sDataItem it; it.addr = addr; it.type = type;
    it.topic = topic; it.subtopic = sub; it.id = id;
    it.value = value; it.rw = 1; it.txType = mTxType;
    return Set_Core::bulid()->setting(it);
}

QString Rpc_Obj::pduGetString(uchar addr, uchar fc, uchar id)
{
    sStrItem it; it.addr = addr;
    it.fc = fc; if(id) id--;
    it.id = id;
    return Set_Core::bulid()->getString(it);
}

bool Rpc_Obj::pduSetString(uchar addr, uchar fc, uchar id, const QString &str)
{
    sStrItem it; it.addr = addr; it.fc = fc; it.id = id; it.rw = 1;
    qstrcpy((char *)it.str, str.toLatin1().data()); it.txType = mTxType;
    return Set_Core::bulid()->setString(it);
}

