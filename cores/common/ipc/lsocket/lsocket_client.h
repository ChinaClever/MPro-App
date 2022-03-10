#ifndef LSOCKET_CLIENT_H
#define LSOCKET_CLIENT_H
#include <QLocalSocket>
#include <QLocalServer>
#include "lsocket_object.h"

class LSocket_Client : public LSocket_Object
{
    Q_OBJECT
public:
    explicit LSocket_Client(QObject *parent = nullptr);
    LSocket_Client(const QString &key, QObject *parent = nullptr);
    QByteArray transLsc(const QByteArray &array, int msec=1000) {return transLsc(array, msec, key());}
    QByteArray transLsc(const QByteArray &array, int msec, const QString &key);

    bool writeLsc(const QByteArray &array) {return writeLsc(array, key());}
    bool writeLsc(const QByteArray &array, const QString &key);
    bool lscReconnect(const QString &ser);
    ~LSocket_Client();

public slots:
    bool lscReconnect() {return lscReconnect(key());}

private:
    void lsClientCreate();

private slots:
    void lsc_connectSlot();
    void lsc_disconnectSlot();

private:
    bool mLsCnted;
    QLocalSocket *mSocketLsc;
};

#endif // LSOCKET_CLIENT_H
