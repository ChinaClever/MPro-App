#ifndef DBUS_SENDER_H
#define DBUS_SENDER_H

#include "dbus_object.h"

class DBus_Sender : public DBus_Object
{
    Q_OBJECT
public:
    explicit DBus_Sender(QObject *parent = nullptr);
    DBus_Sender(const QString &key, QObject *parent = nullptr);
    bool sendBus(const QVariantList &arguments);
    bool sendBus(const QVariant &v);

signals:
    void dbus_send_sig(const QVariantList &);
};

#endif // DBUS_SENDER_H
