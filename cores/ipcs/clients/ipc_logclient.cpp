/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ipc_logclient.h"

IPC_LogClient::IPC_LogClient(QObject *parent) : IPC_ObjClient{parent}
{

}

IPC_LogClient *IPC_LogClient::bulid(QObject *parent)
{
    static IPC_LogClient *sington = nullptr;
    if(!sington) {
        sington = new IPC_LogClient(parent);
        sington->initFunction(IPC_KEY_LOG, true);
    }
    return sington;
}

bool IPC_LogClient::write(eLogs id, const QStringList &value)
{
    QVariantList lv{id, value};
    bool ret = inputCheck(lv);
    if(ret) ret = mDbus->sendBus(lv);
    return ret;
}

QVariant IPC_LogClient::read(eLogs id, int page)
{
    return  readBus(QVariantList {id, page});
}

bool IPC_LogClient::inputCheck(const QVariantList &values)
{
    bool ret = false;
    int id = values.first().toInt();
    if(id <= 6) ret = true;
    return ret;
}

int IPC_LogClient::countLog(eLogs id)
{
    QString str = tr("%1;%2").arg(id).arg(2);
    QByteArray array = mDbus->transLsc(str.toLocal8Bit());;
    return array.toInt();
}

void IPC_LogClient::clearLog(eLogs id)
{
    QString str = tr("%1;%2").arg(id).arg(1);
    mDbus->writeLsc(str.toLocal8Bit());;
}
