#ifndef IPC_LOGSERVER_H
#define IPC_LOGSERVER_H

#include "ipc_cfgserver.h"

class IPC_LogServer : public IPC_ObjServer
{
    Q_OBJECT
    explicit IPC_LogServer(QObject *parent = nullptr);
public:
    static IPC_LogServer *bulid(QObject *parent = nullptr);

public slots:
    void dbus_recv_slot(int id, const QStringList &ls);
    QStringList dbus_reply_slot(int id, int page);

protected:
    QList<const char *> busRecvMethods();
    QByteArray lsRecv(const QByteArray &v);

private:
    QString userRead(int page);
    void userWrite(const QStringList &ls);

    QString opRead(int page);
    void opWrite(const QStringList &ls);

    QString sysRead(int page);
};

#endif // IPC_LOGSERVER_H
