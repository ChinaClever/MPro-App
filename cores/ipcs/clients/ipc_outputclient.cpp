/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ipc_outputclient.h"

IPC_OutputClient::IPC_OutputClient(QObject *parent)
    : IPC_ObjClient{parent}
{

}


IPC_OutputClient *IPC_OutputClient::bulid(QObject *parent)
{
    static IPC_OutputClient *sington = nullptr;
    if(!sington) {
        sington = new IPC_OutputClient(parent);
        sington->initFunction(IPC_KEY_RELAY);
    }
    return sington;
}


bool IPC_OutputClient::ctrl(int addr, int id, int value)
{
    return msgSend(addr, id, 1, value);
}

bool IPC_OutputClient::delaySet(int addr, int id, uchar value)
{
    return msgSend(addr, id, 2, value);
}

bool IPC_OutputClient::swModeSet(int addr, int id, uchar value)
{
    return msgSend(addr, id, 3, value);
}

bool IPC_OutputClient::opNameSet(int addr, int id, const QString &value)
{
    return msgSend(addr, id, 4, value);
}

/**
 * fc == 1 开关控制          fc == ２ 设置延时功能
 * fc == 3 设置形状模式功能　　fc == 4 输出位名称设置
 */
bool IPC_OutputClient::msgSend(int addr, int id, int fc, const QVariant &msg)
{
    QVariantList lv{addr, id, fc, msg.toString()};
    bool ret = inputCheck(lv);
    if(ret) ret = mDbus->sendBus(lv);
    return ret;
}

bool IPC_OutputClient::inputCheck(const QVariantList &values)
{
    bool ret = false;
    int id = values.at(1).toInt();
    int addr = values.first().toInt();
    if((addr <= DEV_NUM) && (id <= OUTPUT_NUM)) ret = true;
    else qDebug() << "Error: IPC Relay Client Parament" << __func__;
    return ret;
}
