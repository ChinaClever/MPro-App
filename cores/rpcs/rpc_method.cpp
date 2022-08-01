/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "rpc_method.h"

Rpc_Method::Rpc_Method(QObject *parent)
    : QObject{parent}
{
    setTxType(DTxType::TxRpc);
}

Rpc_Method *Rpc_Method::bulid(QObject *parent)
{
    static Rpc_Method* sington = nullptr;
    if(sington == nullptr) {
        sington = new Rpc_Method(parent);
    }
    return sington;
}

int Rpc_Method::pduMetaData(uchar addr,  uchar type, uchar topic, uchar sub, uchar id)
{
    //jcon::JsonRpcServer::clientEndpoint()->peerAddress().toString();
    sDataItem *it = &mIt; it->addr = addr; it->type = type; if(id) id--;
    it->topic = topic; it->subtopic = sub; it->id = id;
    it->rw = 0; it->value = 0;
    Set_Core::bulid()->upIndexValue(mIt);
    return mIt.value;
}

bool Rpc_Method::pduSetData(uchar addr,  uchar type, uchar topic, uchar sub, uchar id, uint value, uchar soi)
{
    sDataItem it; it.addr = addr; it.type = type; //it.soi = soi;
    it.topic = topic; it.subtopic = sub; it.id = id;
    it.value = value; it.rw = 1; it.txType = mTxType;
    return Set_Core::bulid()->setting(it);
}

QString Rpc_Method::pduGetString(uchar addr, uchar fc, uchar id)
{
    sCfgItem it; it.addr = addr; it.fc = fc; it.id = id;
    if((SFnCode::OutputName == fc) && id) it.id --;
    return Set_Core::bulid()->getCfg(it);
}

bool Rpc_Method::pduSetString(uchar addr, uchar fc, uchar id, const QString &str)
{
    sCfgItem it; it.addr = addr; it.fc = fc; it.id = id; it.rw = 1; //it.soi = soi;
    qstrcpy((char *)it.str, str.toLatin1().data()); it.txType = mTxType;
    return Set_Core::bulid()->setCfg(it);
}

int Rpc_Method::pduDevCfg(uchar addr, uchar fc, uchar type)
{
    sCfgItem it; it.addr = addr;
    it.fc = fc; it.id = type; it.isDigit = 1;
    return Set_Core::bulid()->getCfg(it).toInt();
}

bool Rpc_Method::pduSetCfg(uchar addr, uchar fc, uchar type, int value)
{
    sCfgItem it; it.addr = addr; it.fc = fc; it.id = type; it.rw = 1;
    it.value = value; it.isDigit = 1; it.txType = mTxType; //it.soi = soi;
    return Set_Core::bulid()->setCfg(it);
}

QString Rpc_Method::pduLogFun(uchar type, uchar fc, int id, int cnt)
{
    sLogFcIt it;
    it.type = type; it.fc = fc;
    it.cnt = cnt; it.id = id;
    return Log_Core::bulid()->log_readFun(it);
}

bool Rpc_Method::pduDevNameSet(int addr, const QString &value)
{
     return pduSetString(addr, SFnCode::Uuts, 6, value);
}

QString Rpc_Method::pduDevNameGet(int addr)
{
    return pduGetString(addr, SFnCode::Uuts, 6);
}

QString Rpc_Method::pduDevSnGet(int addr)
{
    return pduGetString(addr, SFnCode::Uuts, 7);
}

bool Rpc_Method::pduDevSnSet(int addr, const QString &value)
{
    return pduSetString(addr, SFnCode::Uuts, 7, value);
}

bool Rpc_Method::pduQRcodeGenerator(const QString &value, int addr)
{
    return pduSetString(addr, SFnCode::EQRcode, 0, value);
}

QString Rpc_Method::pduOutputNameGet(int addr, int id)
{
    return pduGetString(addr, SFnCode::OutputName, id);
}

QString Rpc_Method::pduGroupingGet(int addr, int id)
{
    return pduGetString(addr, SFnCode::EGrouping, id);
}

QString Rpc_Method::pduGroupNameGet(int addr, int id)
{
    return pduGetString(addr, SFnCode::EGroupName, id);
}

bool Rpc_Method::pduOutputNameSet(int addr, int id, const QString &value)
{
    return pduSetString(addr, SFnCode::OutputName, id, value);
}

bool Rpc_Method::pduGroupNameSet(int addr, int id, const QString &value)
{
    return pduSetString(addr, SFnCode::EGroupName, id, value);
}

bool Rpc_Method::pduGroupingSet(int addr, int id, const QString &value)
{
    return pduSetString(addr, SFnCode::EGrouping, id, value);
}

bool Rpc_Method::pduRelaysCtrl(int addr, int start, int num, uchar on)
{
    uchar topic = DTopic::Relay;
    uchar sub = DSub::Relays;
    uchar type = start;

    return pduSetData(addr, type, topic, sub, num, on);
}

bool Rpc_Method::pduRelayCtrl(int addr, int id, uchar on)
{
    uchar type = DType::Output;
    uchar topic = DTopic::Relay;
    uchar sub = DSub::Value;

    return pduSetData(addr, type, topic, sub, id, on);
}

bool Rpc_Method::pduGroupCtrl(int addr, int id, uchar on)
{
    uchar type = DType::Group;
    uchar topic = DTopic::Relay;
    uchar sub = DSub::Value;

    return pduSetData(addr, type, topic, sub, id, on);
}

uint Rpc_Method::pduRelayStatus(int addr, int id)
{
    uchar type = DType::Output;
    uchar topic = DTopic::Relay;
    uchar sub = DSub::Value;

    return pduMetaData(addr, type, topic, sub, id);
}

bool Rpc_Method::pduRelayModeSet(int addr, int id, uchar mode)
{
    uchar type = DType::Output;
    uchar topic = DTopic::Relay;
    uchar sub = DSub::Rated;

    return pduSetData(addr, type, topic, sub, id, mode);
}

bool Rpc_Method::pduRelayDelaySet(int addr, int id, uchar delay)
{
    uchar type = DType::Output;
    uchar topic = DTopic::Relay;
    uchar sub = DSub::VMax;

    return pduSetData(addr, type, topic, sub, id, delay);
}
