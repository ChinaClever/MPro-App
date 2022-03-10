#include "ipc_settingclient.h"

IPC_SettingClient::IPC_SettingClient(QObject *parent) : IPC_ObjClient{parent}
{

}

IPC_SettingClient *IPC_SettingClient::bulid(QObject *parent)
{
    static IPC_SettingClient *sington = nullptr;
    if(!sington) {
        sington = new IPC_SettingClient(parent);
        sington->initFunction(IPC_KEY_SETTING);
    }
    return sington;
}

bool IPC_SettingClient::setValue(const QString &prefix, const QString &key, const QVariant &value)
{
    QVariantList lv{prefix, key, value};
    bool ret = inputCheck(lv);
    if(ret) ret =  mDbus->sendBus(lv);
    return ret;
}

QVariant IPC_SettingClient::value(const QString &prefix, const QString &key, const QVariant &defaultValue)
{
    return  readBus(QVariantList {prefix, key, defaultValue});
}

bool IPC_SettingClient::inputCheck(const QVariantList &values)
{
    bool ret = false;
    QVariant v = values.last(); int id = v.type();
    if((id == QMetaType::QString) || (id == QMetaType::Int)) ret = true;
    else qDebug() << "Error: IPC Setting Client input Type" << v.typeName();
    return ret;
}
