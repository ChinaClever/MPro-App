/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ipc_outputclient.h"

IPC_OutputClient::IPC_OutputClient(QObject *parent)
    : IPC_EchoClient{parent}
{

}

bool IPC_OutputClient::relaySet(uchar addr, uchar id, uchar fc, uchar value)
{
    uchar type = DType::Output; uchar topic = DTopic::Relay;
    return setting(addr, type, topic, fc, id, value);
}

bool IPC_OutputClient::relayCtrl(uchar addr, uchar id, uchar value)
{
    return relaySet(addr, id, DSub::Value, value);
}

bool IPC_OutputClient::relayDelay(uchar addr, uchar id, uchar value)
{
    return relaySet(addr, id, DSub::VMax, value);
}

bool IPC_OutputClient::relayMode(uchar addr, uchar id, uchar value)
{
    return relaySet(addr, id, DSub::Rated, value);
}

bool IPC_OutputClient::opNameSet(int addr, int id, const QString &value)
{
    return setString(addr, 10, id, value);
}

QString IPC_OutputClient::opName(int addr, int id)
{
    return getString(addr, 10, id);
}
