#include "dtls_sender.h"
#include <thread>

Dtls_Sender::Dtls_Sender(QObject *parent)
    : QObject{parent}
{
    mThread = new CThread(this);
    mThread->init(this, SLOT(run()));
}

bool Dtls_Sender::writeData(Dtls_Association *dtls)
{
    QByteArray array; int max = 999;
    QString name = dtls->getName();
    bool ret = dtls->writeData(mHead);
    if(!ret) qDebug() << "Error: Dtls write head";
    for(int i=0; i<mArray.size();i+=array.size()) {
        array = mArray.mid(i, max);
        ret = dtls->writeData(array);
        if(ret) {
            int v = (i*100.0)/mHosts.size();
            if(0==(i%5)) emit subProgress(name, v);
        } else {
            emit errorMessage(name);
            break;
        }
    }

    return ret;
}

bool Dtls_Sender::workDown(const QString &host)
{
    startNewConnection(host);
    bool ret = mDtls->startHandshake();
    if(ret) {
        ret = writeData(mDtls);
        if(ret) emit subProgress(host, 100);
        else emit infoMessage(ret, tr("%1 Dtls 数据发送失败").arg(host));
    } else emit infoMessage(ret, tr("%1 Dtls 连接失败").arg(host));
    delete mDtls; if(ret) cm::mdelay(1); else cm::mdelay(30);
    return ret;
}

void Dtls_Sender::run()
{
    for(int i=0; i<mHosts.size(); ++i) {
        bool ret = workDown(mHosts.at(i));
        if(!ret) ret = workDown(mHosts.at(i));
        emit finishSig(mHosts.at(i), ret);
        int v = ((i+1)*100.0)/mHosts.size();
        emit progress(v);
    }
}

bool Dtls_Sender::send(const QStringList &ips, const QByteArray &array)
{
    mHosts = ips;
    mArray = array;
    mThread->onceRun();
    return true;
}

bool Dtls_Sender::sendFile(const QStringList &ips, const QString &fn, const sFileTrans &it)
{
    bool ret = false;
    if(File::CheckMd5(fn)) {
        QByteArray array; QDataStream in(&array, QIODevice::WriteOnly); QFile file(fn);
        in << it.fc << it.recvPath << it.file << it.md5 << END_CRC; mHead = array;
        if(file.exists() && file.open(QIODevice::ReadOnly)) {
            ret = send(ips, file.readAll());
        }
    } else qDebug() << "Error: Dtls Sender Md5";
    return ret;
}

void Dtls_Sender::startNewConnection(const QString &address)
{
    mDtls = new Dtls_Association(address);
    connect(mDtls, &Dtls_Association::errorMessage, this, &Dtls_Sender::throwMessage);
    connect(mDtls, &Dtls_Association::warningMessage, this, &Dtls_Sender::throwMessage);
    connect(mDtls, &Dtls_Association::infoMessage, this, &Dtls_Sender::throwMessage);
    // connect(newConnection.data(), &DtlsAssociation::serverResponse, this, &Dtls_Sender::addServerResponse);
}

void Dtls_Sender::throwMessage(const QString &message)
{
    qDebug() << message;
}
