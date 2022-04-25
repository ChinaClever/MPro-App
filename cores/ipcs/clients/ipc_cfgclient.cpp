/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ipc_cfgclient.h"

IPC_CfgClient::IPC_CfgClient(QObject *parent) : IPC_ObjClient{parent}
{

}

IPC_CfgClient *IPC_CfgClient::bulid(QObject *parent)
{
    static IPC_CfgClient *sington = nullptr;
    if(!sington) {
        sington = new IPC_CfgClient(parent);
        sington->initFunction(IPC_KEY_SETTING);
    }
    return sington;
}

bool IPC_CfgClient::setValue(const QString &prefix, const QString &key, const QVariant &value)
{
    QVariantList lv{prefix, key, value.toString()};
    bool ret = inputCheck(lv);
    if(ret) ret = mDbus->sendBus(lv);
    return ret;
}

QVariant IPC_CfgClient::value(const QString &prefix, const QString &key, const QVariant &defaultValue)
{
    return  readBus(QVariantList {prefix, key, defaultValue.toString()});
}

bool IPC_CfgClient::inputCheck(const QVariantList &values)
{
    bool ret = false;
    QVariant v = values.last(); int id = v.type();
    if((id == QMetaType::QString) || (id == QMetaType::Int)) ret = true;
    else qDebug() << "Error: IPC Setting Client input Type" << v.typeName();
    return ret;
}
