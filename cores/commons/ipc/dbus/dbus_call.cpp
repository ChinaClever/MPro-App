/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "dbus_call.h"

DBus_Call::DBus_Call(QObject *parent) : DBus_Sender{parent}
{

}

DBus_Call::DBus_Call(const QString &key, QObject *parent) : DBus_Sender{parent}
{
    setKey(key);
}

QVariantList DBus_Call::callBus(const QVariant &v)
{
    return callBus(QVariantList{v});
}

QVariantList DBus_Call::callBus(const QVariantList &arguments)
{
    QVariantList res;
    QDBusMessage msg = QDBusMessage::createMethodCall(mService, mBusPath, mInterface, "dbus_reply_slot");
    msg.setArguments(arguments); QDBusMessage response = mBus.call(msg);
    if (response.type() == QDBusMessage::ReplyMessage) {
        res = response.arguments(); //for(auto i:res) qDebug() << i.typeName() << i;
    } else throwError(Q_FUNC_INFO);
    return res;
}
