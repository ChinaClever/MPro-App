/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ipc_relayclient.h"

IPC_RelayClient::IPC_RelayClient(QObject *parent)
    : IPC_ObjClient{parent}
{

}


IPC_RelayClient *IPC_RelayClient::bulid(QObject *parent)
{
    static IPC_RelayClient *sington = nullptr;
    if(!sington) {
        sington = new IPC_RelayClient(parent);
        sington->initFunction(IPC_KEY_RELAY);
    }
    return sington;
}


bool IPC_RelayClient::ctrl(int addr, int id, int value)
{
    QVariantList lv{addr, id, value};
    bool ret = inputCheck(lv);
    if(ret) ret = mDbus->sendBus(lv);
    return ret;
}


bool IPC_RelayClient::inputCheck(const QVariantList &values)
{
    bool ret = false;
    int id = values.at(1).toInt();
    int addr = values.first().toInt();
    if((addr <= DEV_NUM) && (id <= OUTPUT_NUM)) ret = true;
    else qDebug() << "Error: IPC Relay Client Parament" << __func__;
    return ret;
}
