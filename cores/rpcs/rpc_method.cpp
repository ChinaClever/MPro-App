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

// dc 0 使用系统配置， 1读的参数 2 所有参数
QString Rpc_Method::pduMetaData(int addr, int dc)
{
    return Integr_JsonBuild::bulid()->getJson(addr, dc);
}

QString Rpc_Method::execute(const QString &cmd)
{
    return cm::execute(cmd);
}

QString Rpc_Method::pduLogHda(const QString &start, const QString &end, int addr, int type, int topic, int index)
{
    sLogHdaIt it; it.addr = addr; it.type = type;
    it.topic = topic; it.index = index;
    it.start = start; it.end = end;
    return Log_Core::bulid()->log_readHda(it);
}

double Rpc_Method::pduDataGet(int addr,  int type, int topic, int sub, int id)
{
    //jcon::JsonRpcServer::clientEndpoint()->peerAddress().toString();
    sDataItem *it = &mIt; it->addr = addr; it->type = type; if(id) id--;
    it->topic = topic; it->subtopic = sub; it->id = id;
    it->rw = 0; it->value = 0; Set_Core::bulid()->upMetaData(mIt);
    //cout << addr << type << topic << sub << id << mIt.value << mIt.value / cm::decimal(mIt);
    return mIt.value / cm::decimal(mIt);
}

int Rpc_Method::pduDataSet(int addr,  int type, int topic, int sub, int id, double value)
{
    sDataItem it; it.addr = addr; it.type = type;
    it.topic = topic; it.subtopic = sub; it.id = id; it.rw = 1;
    it.value = value * cm::decimal(it); it.txType = mTxType;
    //cout << addr << type << topic << sub << id << it.value << it.value * cm::decimal(it);
    return Set_Core::bulid()->setting(it);
}

int Rpc_Method::pduCfgSet(int type, int fc, const QVariant &value, int id, int addr)
{
    sCfgItem it; it.addr = addr; it.type = type;
    it.txType = mTxType; it.fc = fc; it.id = id;
    return Set_Core::bulid()->setCfg(it, value);
}

QString Rpc_Method::pduCfgGet(int type, int fc, int id, int addr)
{
    sCfgItem it; it.addr = addr; it.type = type; it.fc = fc; it.id = id;
    return Set_Core::bulid()->getCfg(it).toString();
}

QString Rpc_Method::pduLogFun(int type, int fc, int id, int cnt)
{
    sLogFcIt it;
    it.type = type; it.fc = fc;
    it.cnt = cnt; it.id = id;
    return Log_Core::bulid()->log_readFun(it);
}

bool Rpc_Method::pduRelaysCtrl(int addr, int start, int num, int on)
{
    uchar topic = DTopic::Relay;
    uchar sub = DSub::Relays;
    uchar type = start;

    return pduDataSet(addr, type, topic, sub, num, on);
}

