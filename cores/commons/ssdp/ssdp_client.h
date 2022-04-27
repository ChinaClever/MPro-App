#ifndef SSDP_CLIENT_H
#define SSDP_CLIENT_H

#include <QtCore>
#include <QUdpSocket>
#include <QHostAddress>
#include "commons.h"

class Ssdp_Client : public QObject
{
    Q_OBJECT
public:
    explicit Ssdp_Client(QObject *parent = nullptr);
    static Ssdp_Client *bulid(QObject *parent = nullptr);

protected:
    bool write(const QVariant &var);
    void recvMsg(const QByteArray &array);
    bool rplySearchTarget(const QByteArray &array);

protected slots:
    virtual void readMsgSlot();

protected:
    int mPort;
    QUdpSocket *mSocket;
    QHostAddress mAddress;
};

#endif // SSDP_CLIENT_H
