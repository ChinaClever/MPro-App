/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ipc_logclient.h"

IPC_LogClient::IPC_LogClient(QObject *parent) : IPC_EchoClient{parent}
{

}

bool IPC_LogClient::log_msgSend(const sIpcLog &msg)
{
    QVariantList lv{6, cm::toByteArray(msg)};
    bool ret = inputCheck(lv);
    if(ret) ret = sendSocket(lv);
    return ret;
}

QString IPC_LogClient::log_get(const sIpcLog &unit)
{
    int t = 100; if(unit.fc == 2) t = 300;
    QByteArray array = cm::toByteArray(unit);    
    QString res = readSocket(QVariantList {6, array}, t).toString();
    return res;
}

QVariant IPC_LogClient::log_fun(const sIpcLog &it)
{
    if(3 == it.fc) return log_msgSend(it);
    else return log_get(it);
}

QString IPC_LogClient::log_read(int id, int page, int noe)
{
   sIpcLog it;
   it.id = id;
   it.fc = 2;
   it.noe = noe;
   it.page = page;
   return log_get(it);
}

int IPC_LogClient::log_counts(int id)
{
    sIpcLog it;
    it.id = id;
    it.fc = 1;
    return log_get(it).toInt();
}

bool IPC_LogClient::log_clear(int id)
{
    sIpcLog it;
    it.id = id;
    it.fc = 3;
    return log_msgSend(it);
}
