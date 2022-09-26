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
    return Integr_JsonBuild::bulid()->getJson(addr);
}

QString Rpc_Method::execute(const QString &cmd)
{
    return cm::execute(cmd);
}

double Rpc_Method::getDecimal(const sDataItem &it)
{
    double res = 1; switch (it.topic) {
    case DTopic::Vol: res = COM_RATE_VOL; break;
    case DTopic::Cur: res = COM_RATE_CUR; break;
    case DTopic::Pow: res = COM_RATE_POW; break;
    case DTopic::Ele: res = COM_RATE_ELE; break;
    case DTopic::PF: res = COM_RATE_PF; break;
    case DTopic::ArtPow: res = COM_RATE_POW; break;
    case DTopic::ReactivePow: res = COM_RATE_POW; break;
    case DTopic::Tem: res = COM_RATE_TEM; break;
    case DTopic::Hum: res = COM_RATE_HUM; break;
    default: cout << Q_FUNC_INFO; break;
    }

    if((DSub::Size==it.subtopic) || (DSub::Alarm==it.subtopic) || (DSub::EnAlarm==it.subtopic)) res = 1;

    return res;
}

double Rpc_Method::pduDataGet(uchar addr,  uchar type, uchar topic, uchar sub, uchar id)
{
    //jcon::JsonRpcServer::clientEndpoint()->peerAddress().toString();
    sDataItem *it = &mIt; it->addr = addr; it->type = type; if(id) id--;
    it->topic = topic; it->subtopic = sub; it->id = id;
    it->rw = 0; it->value = 0; Set_Core::bulid()->upMetaData(mIt);
    return mIt.value / getDecimal(mIt);
}

bool Rpc_Method::pduDataSet(uchar addr,  uchar type, uchar topic, uchar sub, uchar id, double value)
{
    sDataItem it; it.addr = addr; it.type = type;
    it.topic = topic; it.subtopic = sub; it.id = id; it.rw = 1;
    it.value = value * getDecimal(mIt); it.txType = mTxType;
    return Set_Core::bulid()->setting(it);
}

bool Rpc_Method::pduCfgSet(uchar type, uchar fc, const QVariant &value, uchar id, uchar addr)
{
    sCfgItem it; it.addr = addr; it.type = type;
    it.txType = mTxType; it.fc = fc; it.id = id;
    return Set_Core::bulid()->setCfg(it, value);
}

QString Rpc_Method::pduCfgGet(uchar type, uchar fc, uchar id, uchar addr)
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

