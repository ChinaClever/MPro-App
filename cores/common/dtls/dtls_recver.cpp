#include "dtls_recver.h"

Dtls_Recver::Dtls_Recver(QObject *parent)
    : QObject{parent}
{
    mFile = new QFile;
    mDtls = new Dtls_Service(this);
    connect(mDtls, &Dtls_Service::errorMessage, this, &Dtls_Recver::throwError);
    connect(mDtls, &Dtls_Service::warningMessage, this, &Dtls_Recver::throwMessage);
    connect(mDtls, &Dtls_Service::infoMessage, this, &Dtls_Recver::throwMessage);
    connect(mDtls, &Dtls_Service::datagramReceived, this, &Dtls_Recver::rcvClientMessage);
    mDtls->listen();
}

bool Dtls_Recver::setFile(const QString &fn)
{
    isFinshed = false;
    mFile->setFileName(fn);
    bool ret = mFile->open(QIODevice::WriteOnly | QIODevice::Truncate);
    if(ret) mSize = 0; else throwMessage(tr("Error: Dtls Recver open file").arg(fn));
    return ret;
}

int Dtls_Recver::waitForFinish()
{
    while(!isFinshed) mdelay();
    return mSize;
}

void Dtls_Recver::throwError(const QString &message)
{
    if(mFile->isOpen()) mFile->close();
    throwMessage(message);
    emit finishSig(mSize);
    isFinshed = true;
}

void Dtls_Recver::throwMessage(const QString &message)
{
    qDebug() << message;
}

void Dtls_Recver::rcvClientMessage(const QByteArray &data)
{
    if(mFile->isWritable()) {
        mFile->write(data);
        mSize += data.size();
    } else throwMessage(tr("Error: Dtls Recver write file %1").arg(data.data()));
}
