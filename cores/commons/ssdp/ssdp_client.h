#ifndef SSDP_CLIENT_H
#define SSDP_CLIENT_H

#include <QtCore>
#include <QUdpSocket>
#include <QHostAddress>
#include "commons.h"

struct sSdpIt{
    int fc;
    int version;
    QString describe;
    QByteArray array;
    ushort crc;
};

class Ssdp_Client : public QObject
{
    Q_OBJECT
    explicit Ssdp_Client(QObject *parent = nullptr);
public:
    static Ssdp_Client *bulid(QObject *parent = nullptr);

signals:
    void recvSig(uchar fc, const QString &, const QByteArray&);

private:
    bool write(const QVariant &var);
    void recvMsg(QByteArray &array);
    bool rplySearchTarget(const QString &room, const QByteArray &array);

private slots:
    virtual void readMsgSlot();

private:
    int mPort;
    QUdpSocket *mSocket;
    QHostAddress mAddress;
};

#endif // SSDP_CLIENT_H
