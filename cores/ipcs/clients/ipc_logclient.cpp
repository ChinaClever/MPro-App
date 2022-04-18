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

bool IPC_LogClient::write(int id, const QStringList &value)
{
    QVariantList lv{id, value};
    bool ret = inputCheck(lv);
    if(ret) ret = mDbus->sendBus(lv);
    return ret;
}

QVariant IPC_LogClient::read(int id, int page)
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
