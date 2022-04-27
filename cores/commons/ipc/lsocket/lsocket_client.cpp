/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "lsocket_client.h"

LSocket_Client::LSocket_Client(QObject *parent) : LSocket_Object{parent}
{
    mLsCnted = false;
    mSocketLsc = nullptr;
}

LSocket_Client::LSocket_Client(const QString &key, QObject *parent) : LSocket_Object{parent}
{
    setKey(key);
    mLsCnted = false;
    mSocketLsc = nullptr;
}

LSocket_Client::~LSocket_Client()
{
    if(mSocketLsc) mSocketLsc->close();
}

void LSocket_Client::lsClientCreate()
{
    if(!mSocketLsc) {
        mSocketLsc = new QLocalSocket(this);
        connect(mSocketLsc,SIGNAL(connected()),this,SLOT(lsc_connectSlot()));  //数据接收
        connect(mSocketLsc,SIGNAL(disconnected()),this,SLOT(lsc_disconnectSlot()));  //连接断开
    }
}

bool LSocket_Client::lscReconnect(const QString &ser)
{
    bool ret = false;
    if(ser.size()) lsClientCreate(); else return ret;
    if((ser != key()) || !mLsCnted) {
        mSocketLsc->disconnectFromServer();
        mSocketLsc->connectToServer(ser);
        ret = mSocketLsc->waitForConnected(1000);
        if(ret) setKey(ser); else {
            qDebug() << mSocketLsc->errorString();
            QTimer::singleShot(1000, this, SLOT(lscReconnect()));
        }
    } else ret = true;

    return ret;
}

bool LSocket_Client::writeLsc(const QByteArray &array, const QString &key)
{
    bool ret = lscReconnect(key);
    if(mLsCnted && mSocketLsc->isWritable()) {
        ret = mSocketLsc->write(array); mSocketLsc->flush();
    } else qDebug() << "Error:" << Q_FUNC_INFO << mSocketLsc->errorString() << array;

    return ret;
}

QByteArray LSocket_Client::transLsc(const QByteArray &array, int msec, const QString &key)
{
    QByteArray rcv;
    if(writeLsc(array, key) && mSocketLsc->isReadable()) {
        for(int i=0; i<msec; i+=10) {
            cm::mdelay(10); rcv = mSocketLsc->readAll();
            if(rcv.size()) break;
        }
    }

    return rcv;
}

void LSocket_Client::lsc_connectSlot()
{
    mLsCnted = true; //qDebug()<< tr("连接服务端成功!");
}

void LSocket_Client::lsc_disconnectSlot()
{
    mLsCnted = false; mSocketLsc->close();
    QTimer::singleShot(1000, this, SLOT(lscReconnect()));
    qDebug() << "Error:" << Q_FUNC_INFO << mSocketLsc->errorString();
}

