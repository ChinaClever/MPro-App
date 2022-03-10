#ifndef SSDP_SERVER_H
#define SSDP_SERVER_H

#include "ssdp_client.h"

class Ssdp_Server : public Ssdp_Client
{
    Q_OBJECT
public:
    explicit Ssdp_Server(QObject *parent = nullptr);
    static Ssdp_Server *bulid(QObject *parent = nullptr);

    bool write(const QVariant &var);
    QStringList searchTarget();

protected:
    QStringList respondList(const QByteArray &key);

private:
    int mPort;
    QUdpSocket *mSocket;
    QHostAddress mAddress;
};

#endif // SSDP_SERVER_H
