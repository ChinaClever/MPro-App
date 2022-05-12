/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "dbus_sender.h"

DBus_Sender::DBus_Sender(QObject *parent) : DBus_Object{parent}
{

}

DBus_Sender::DBus_Sender(const QString &key, QObject *parent) : DBus_Object{parent}
{
    setKey(key);
}

bool DBus_Sender::sendBus(const QVariant &v)
{
    return sendBus(QVariantList{v});
}

bool DBus_Sender::sendBus(const QVariantList &arguments)
{
    QDBusMessage message = QDBusMessage::createSignal(mBusPath, mInterface, mBusSig);
    message.setArguments(arguments); bool ret = mBus.send(message); if(!ret) throwError("send");
    return ret;
}
