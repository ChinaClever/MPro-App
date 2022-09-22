/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "rpc_method.h"
#include "integr_core.h"

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

QString Rpc_Method::pduMetaData(uchar addr)
{
    uchar dc = Integr_Core::pushCfg.dataContent;
    return Integr_JsonBuild::bulid()->getJson(addr, dc);
}

QString Rpc_Method::execute(const QString &cmd)
{
    return cm::execute(cmd);
}

int Rpc_Method::pduDataGet(uchar addr,  uchar type, uchar topic, uchar sub, uchar id)
{
    //jcon::JsonRpcServer::clientEndpoint()->peerAddress().toString();
    sDataItem *it = &mIt; it->addr = addr; it->type = type; if(id) id--;
    it->topic = topic; it->subtopic = sub; it->id = id;
    it->rw = 0; it->value = 0;
    Set_Core::bulid()->upMetaData(mIt);
    return mIt.value;
}

bool Rpc_Method::pduDataSet(uchar addr,  uchar type, uchar topic, uchar sub, uchar id, uint value)
{
    sDataItem it; it.addr = addr; it.type = type;
    it.topic = topic; it.subtopic = sub; it.id = id;
    it.value = value; it.rw = 1; it.txType = mTxType;
    return Set_Core::bulid()->setting(it);
}

bool Rpc_Method::pduCfgSet(uchar type, uchar fc, const QVariant &value, uchar addr, uchar id)
{
    sCfgItem it; it.addr = addr; it.type = type;
    it.txType = mTxType; it.fc = fc; it.id = id;
    return Set_Core::bulid()->setCfg(it, value);
}

QString Rpc_Method::pduCfgGet(uchar type, uchar fc, uchar addr, uchar id)
{
    sCfgItem it; it.addr = addr; it.type = type; it.fc = fc; it.id = id;
    return Set_Core::bulid()->getCfg(it).toString();
}

QString Rpc_Method::pduLogFun(uchar type, uchar fc, int id, int cnt)
{
    sLogFcIt it;
    it.type = type; it.fc = fc;
    it.cnt = cnt; it.id = id;
    return Log_Core::bulid()->log_readFun(it);
}

bool Rpc_Method::pduRelaysCtrl(int addr, int start, int num, uchar on)
{
    uchar topic = DTopic::Relay;
    uchar sub = DSub::Relays;
    uchar type = start;

    return pduDataSet(addr, type, topic, sub, num, on);
}

