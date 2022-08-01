/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ipc_logclient.h"

IPC_LogClient::IPC_LogClient(QObject *parent) : IPC_ObjClient{parent}
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
    int t = 20; if(unit.fc == eLog_read) t = 50;
    QByteArray array = cm::toByteArray(unit);    
    QString res = readSocket(QVariantList {6, array}, t).toString();
    return res;
}

QVariant IPC_LogClient::log_fun(uchar type, uchar fc, uint id, uint cnt)
{
     sLogFcIt it;
     it.type = type; it.fc = fc;
     it.cnt = cnt; it.id = id;
    if(eLog_clear == it.fc) return log_msgSend(it);
    else return log_get(it);
}

