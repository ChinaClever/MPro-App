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
    return pduGetString(addr, SFnCode::OutputName, id);
}

bool Rpc_Output::pduOutputNameSet(int addr, int id, const QString &value)
{
    return pduSetString(addr, SFnCode::OutputName, id, value);
}

bool Rpc_Output::pduRelaysCtrl(int addr, int start, int num, uchar on)
{
    uchar topic = DTopic::Relay;
    uchar sub = DSub::Relays;
    uchar type = start;

    return pduSetData(addr, type, topic, sub, num, on);
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

bool Rpc_Output::pduRelayModeSet(int addr, int id, uchar mode)
{
    uchar type = DType::Output;
    uchar topic = DTopic::Relay;
    uchar sub = DSub::Rated;

    return pduSetData(addr, type, topic, sub, id, mode);
}

bool Rpc_Output::pduRelayDelaySet(int addr, int id, uchar delay)
{
    uchar type = DType::Output;
    uchar topic = DTopic::Relay;
    uchar sub = DSub::VMax;

    return pduSetData(addr, type, topic, sub, id, delay);
}
