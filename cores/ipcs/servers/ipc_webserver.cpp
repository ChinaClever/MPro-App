/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ipc_webserver.h"

IPC_WebServer::IPC_WebServer(QObject *parent)
    : IPC_ObjServer{parent}
{

}

IPC_WebServer *IPC_WebServer::bulid(QObject *parent)
{
    static IPC_WebServer *sington = nullptr;
    if(!sington) {
        sington = new IPC_WebServer(parent);
        sington->initFunction(IPC_KEY_WEB);
    }
    return sington;
}

QList<const char *> IPC_WebServer::busRecvMethods()
{
    QList<const char *> res;
    res << SLOT(dbus_recv_slot(int, QByteArray));
    return res;
}

QString IPC_WebServer::dbus_reply_slot(int fc, const QByteArray &array)
{
    QString res;
    if(1 == fc) {
        sDataItem unit = cm::toStruct<sDataItem>(array);
        bool ret = Set_Core::bulid()->upIndexValue(unit);
        if(ret) res = QString::number(unit.value);
    } else if(2 == fc) {
        sStrItem unit = cm::toStruct<sStrItem>(array);
        res = Set_Core::bulid()->getString(unit);
    }

    return res;
}

void IPC_WebServer::dbus_recv_slot(int fc, const QByteArray &array)
{
    if(1 == fc) {
        sDataItem unit = cm::toStruct<sDataItem>(array);
        qDebug() << "GGGGGGGGGG" << unit.value;
        Set_Core::bulid()->setting(unit);
    } else if(2 == fc) {
        sStrItem unit = cm::toStruct<sStrItem>(array);
        Set_Core::bulid()->setString(unit);
    }


}
