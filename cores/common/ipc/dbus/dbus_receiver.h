#ifndef DBUS_RECEIVER_H
#define DBUS_RECEIVER_H

#include "dbus_call.h"

class DBus_Receiver : public DBus_Call
{
    Q_OBJECT
public:
    explicit DBus_Receiver(QObject *parent = nullptr);
    bool busConnects();

protected:
    virtual QList<const char *> busRecvMethods() = 0;

public slots:
    // void dbus_recv_slot(const T &v ...);
};

#endif // DBUS_RECEIVER_H
