#include "ws_object.h"

WS_Object::WS_Object(QObject *parent)
    : QObject{parent}
{
    isRecved = false;
    mSocket = nullptr;
    isConnected = false;
}

void WS_Object::initSocket(QWebSocket *socket)
{
    mSocket = socket;
    mSocket->setParent(parent());
    connect(mSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(mSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(mSocket, SIGNAL(textMessageReceived(QString)), this, SLOT(textMessageReceived(QString)));
    connect(mSocket, SIGNAL(binaryMessageReceived(QByteArray)), this, SLOT(binaryMessageReceived(QByteArray)));
    connect(mSocket, QOverload<const QList<QSslError>&>::of(&QWebSocket::sslErrors),this, &WS_Object::onSslErrors);
}

void WS_Object::close()
{
    isConnected = false;
    if(mSocket) mSocket->close();
}

QVariant WS_Object::recvMessage(int msec)
{
    QVariant rcv;
    for(int i=0; i<msec; i+=10) {
        if(isRecved){rcv = mRecvVar; break;} else cm::mdelay(10);
    } isRecved = false;

    return rcv;
}

QVariant WS_Object::transmit(const QVariant &var, int msecs)
{
    QVariant rcv;
    bool ret = sendMessage(var);
    if(ret) rcv = recvMessage(msecs);
    return rcv;
}

bool WS_Object::sendMessage(const QVariant &var)
{
    bool ret = false; int rtn = 0;
    if(isConnected && mSocket) {
        if(var.type() == QVariant::String) {
            rtn = mSocket->sendTextMessage(var.toString());
        } else if(var.type() == QVariant::BitArray) {
            rtn = mSocket->sendBinaryMessage(var.toByteArray());
        }

        if(rtn > 0) ret = true;
        mSocket->flush();
    }

    return ret;
}

bool WS_Object::sendTextMessage(const QString &message)
{
    return sendMessage(message);
}

bool WS_Object::sendBinaryMessage(const QByteArray &data)
{
    return sendMessage(data);
}

void WS_Object::connected()
{
    isConnected = true;
}

void WS_Object::disconnected()
{
    isConnected = false;
}

void WS_Object::textMessageReceived(const QString &message)
{
    mRecvVar = message;
    isRecved = true;
}

void WS_Object::binaryMessageReceived(const QByteArray &message)
{
    mRecvVar = message;
    isRecved = true;
}

void WS_Object::onSslErrors(const QList<QSslError> &errors)
{
    Q_UNUSED(errors);

    // WARNING: Never ignore SSL errors in production code.
    // The proper way to handle self-signed certificates is to add a custom root
    // to the CA store.

    mSocket->ignoreSslErrors();
}
