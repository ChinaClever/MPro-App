#ifndef WS_OBJECT_H
#define WS_OBJECT_H

#include <QtCore>
#include <QWebSocket>
#include "cthread.h"

class WS_Object : public QObject
{
    Q_OBJECT
public:
    explicit WS_Object(QObject *parent = nullptr);
    void close();
    void initSocket(QWebSocket *socket);
    bool sendMessage(const QVariant &var);
    QVariant recvMessage(int msec=1000);
    bool sendTextMessage(const QString &message);
    bool sendBinaryMessage(const QByteArray &data);
    QVariant transmit(const QVariant &var, int msecs=1000);

protected slots:
    void connected();
    void disconnected();
    void onSslErrors(const QList<QSslError> &errors);
    virtual void textMessageReceived(const QString &message);
    virtual void binaryMessageReceived(const QByteArray &message);

protected:
    bool isRecved;
    bool isConnected;
    QVariant mRecvVar;
    QWebSocket *mSocket;
};

#endif // WS_OBJECT_H
