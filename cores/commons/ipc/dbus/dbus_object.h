#ifndef DBUS_OBJECT_H
#define DBUS_OBJECT_H

#include <QtDBus>
#include "lsocket_client.h"

#define DBUS_SERVICE_NAME   "com.clever.pdu"

class DBus_Object : public LSocket_Client
{
    Q_OBJECT
public:
    explicit DBus_Object(QObject *parent = nullptr);
    virtual void setKey(const QString &newKey) override;
    static bool registerBusService();
    bool registerBusObject();

protected:
    void throwError(const QString &msg);

private:
    static QDBusConnection busConnection();

protected:
    QString mService, mBusPath, mInterface, mBusSig;
    QDBusConnection mBus{busConnection()};
};

#endif // DBUS_OBJECT_H
