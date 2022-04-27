#ifndef DBUS_CALL_H
#define DBUS_CALL_H

#include "dbus_sender.h"

class DBus_Call : public DBus_Sender
{
    Q_OBJECT
public:
    explicit DBus_Call(QObject *parent = nullptr);
    DBus_Call(const QString &key, QObject *parent = nullptr);
    QVariantList callBus(const QVariantList &arguments);
    QVariantList callBus(const QVariant &v);

public slots:
    // R dbus_reply_slot(const T &v ...);
};

#endif // DBUS_CALL_H
