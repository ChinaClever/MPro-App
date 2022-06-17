#ifndef SSDP_SERVER_H
#define SSDP_SERVER_H

#include "ssdp_client.h"

class Ssdp_Server : public QObject
{
    Q_OBJECT
public:
    explicit Ssdp_Server(QObject *parent = nullptr);
    QStringList searchTarget(const QString &room="all");
    bool send(int fc, const QString &describe, const QByteArray &data);
    bool bind();

signals:
    void targetSig(const QString &);

private:
    bool write(const QVariant &var);
    QStringList respondList(const QByteArray &key);

private:
    int mPort;
    QUdpSocket *mSocket;
    QHostAddress mAddress;
};

#endif // SSDP_SERVER_H
