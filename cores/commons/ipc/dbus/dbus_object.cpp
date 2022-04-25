/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "dbus_object.h"

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
             << QDBusConnection::sessionBus().lastError();
}

bool DBus_Object::registerBusService()
{
    QString service = DBUS_SERVICE_NAME;
    QDBusConnection::sessionBus().unregisterService(service);
    bool ret = QDBusConnection::sessionBus().registerService(service);
    if(!ret) qDebug() << "Err: DBus register Service: " << service;
    return ret;
}

bool DBus_Object::registerBusObject()
{
    bool ret = mBus.registerObject(mBusPath, mInterface, this,
               QDBusConnection::ExportAllSlots | QDBusConnection::ExportAllSignals);
    if(!ret) throwError(Q_FUNC_INFO);
    return ret;
}

