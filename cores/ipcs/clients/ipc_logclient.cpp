/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ipc_logclient.h"

IPC_LogClient::IPC_LogClient(QObject *parent) : IPC_EchoClient{parent}
{

}

bool IPC_LogClient::log_msgSend(const sLogFcIt &msg)
{
    QVariantList lv{6, cm::toByteArray(msg)};
    bool ret = inputCheck(lv);
    if(ret) ret = sendSocket(lv);
    return ret;
}

QString IPC_LogClient::log_get(const sLogFcIt &unit)
{
    int t = 20; if(unit.fc == 2) t = 300;
    QByteArray array = cm::toByteArray(unit);    
    QString res = readSocket(QVariantList {6, array}, t).toString();
    return res;
}

QVariant IPC_LogClient::log_fun(uchar type, uchar fc, uint id, uchar noe)
{
     sLogFcIt it;
     it.type = type; it.fc = fc;
     it.noe = noe; it.id = id;
    if(4 == it.fc) return log_msgSend(it);
    else return log_get(it);
}

