/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "rpc_output.h"

Rpc_Output::Rpc_Output(QObject *parent)
    : Rpc_Info{parent}
{

}

QString  Rpc_Output::pduOutputNameGet(int addr, int id)
{
    return pduGetString(addr, 10, id);
}

bool Rpc_Output::pduOutputNameSet(int addr, int id, const QString &value)
{
    return pduSetString(addr, 10, id, value);
}

bool Rpc_Output::pduRelayCtrl(int addr, int id, uchar on)
{
    uchar type = DType::Output;
    uchar topic = DTopic::Relay;
    uchar sub = DSub::Value;

    return pduSetData(addr, type, topic, sub, id, on);
}

uint Rpc_Output::pduRelayStatus(int addr, int id)
{
    uchar type = DType::Output;
    uchar topic = DTopic::Relay;
    uchar sub = DSub::Value;

    return pduMetaData(addr, type, topic, sub, id);
}
