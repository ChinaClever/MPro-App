#include "ipc_delayclient.h"

IPC_DelayClient::IPC_DelayClient(QObject *parent)
    : IPC_ObjClient{parent}
{

}

IPC_DelayClient *IPC_DelayClient::bulid(QObject *parent)
{
    static IPC_DelayClient *sington = nullptr;
    if(!sington) {
        sington = new IPC_DelayClient(parent);
        sington->initFunction(IPC_KEY_DELAY);
    }
    return sington;
}


bool IPC_DelayClient::setDelay(int addr, int id, uchar mode, uchar delay)
{
    QVariantList lv{addr, id, mode, delay};
    bool ret = inputCheck(lv);
    if(ret) ret = mDbus->sendBus(lv);
    return ret;
}


bool IPC_DelayClient::inputCheck(const QVariantList &values)
{
    bool ret = false;
    int id = values.at(1).toInt();
    int addr = values.first().toInt();
    if((addr <= DEV_NUM) && (id <= OUTPUT_NUM)) ret = true;
    else qDebug() << "Error: IPC Delay Client Parament" << __func__;
    return ret;
}
