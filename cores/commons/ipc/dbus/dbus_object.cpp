/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "dbus_object.h"
#define IPC_PC  1

DBus_Object::DBus_Object(QObject *parent) : LSocket_Client{parent}
{
}

void DBus_Object::setKey(const QString &newKey)
{
    mKeyObj = newKey;
    mBusPath = "/" + key();
    mBusSig = "dbus_send_sig";
    mService = DBUS_SERVICE_NAME;
    mInterface = mService + "." + key();
}

void DBus_Object::throwError(const QString &msg)
{
    qDebug() << "Error: D-Bus" << msg << mInterface
             << busConnection().lastError();
}

QDBusConnection DBus_Object::busConnection()
{
#if IPC_PC
    return QDBusConnection::sessionBus();
#else
    return QDBusConnection::systemBus();
#endif
}

bool DBus_Object::registerBusService()
{
#if IPC_PC
    QString service = DBUS_SERVICE_NAME;
    busConnection().unregisterService(service);
    bool ret = busConnection().registerService(service);
#else
    bool ret = busConnection().isConnected();
#endif
    if(!ret) qDebug() << "Err: DBus register Service: " << busConnection().name() <<Q_FUNC_INFO;
    return ret;
}

bool DBus_Object::registerBusObject()
{
    bool ret = mBus.registerObject(mBusPath, this,
                                   QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals);
    if(!ret) throwError(Q_FUNC_INFO);
    return ret;
}

