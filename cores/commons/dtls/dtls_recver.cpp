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
    mDtls->listen(); isFinshed = false;
}


Dtls_Recver *Dtls_Recver::bulid(QObject *parent)
{
    Dtls_Recver* sington = nullptr;
    if(sington == nullptr) {
        sington = new Dtls_Recver(parent);
    }
    return sington;
}

bool Dtls_Recver::setFile(const QString &fn)
{    
    mFile->setFileName(fn); isFinshed = false;
    bool ret = mFile->open(QIODevice::WriteOnly | QIODevice::Truncate);
    if(ret) mSize = 0; else throwMessage(tr("Error: Dtls Recver open file").arg(fn));
    return ret;
}

bool Dtls_Recver::waitForFinish()
{
    while(!isFinshed) cm::mdelay(2);
    return recvFinish();
}

void Dtls_Recver::throwError(const QString &message)
{
    throwMessage(message);
    bool ret = recvFinish();
    emit finishSig(mIt, ret);
}

void Dtls_Recver::throwMessage(const QString &message)
{
    emit messageSig(message);
    qDebug() << message;
}

bool Dtls_Recver::recvFinish()
{
    isFinshed = true;  mSize = 0;
    if(mFile->isOpen()) mFile->close();
    return File::CheckMd5(mIt);
}

bool Dtls_Recver::initFile(const QByteArray &array)
{
    QByteArray rcv(array);
    bool ret = false; sFileTrans *it = &mIt;
    QDataStream out(&rcv, QIODevice::ReadOnly);
    out >> it->fc >> it->dev >> it->path >> it->file >> it->md5 >> it->crc;
    if(it->crc == END_CRC) ret = setFile(it->path + it->file);
    else qDebug() << "Error: Dtls recver head" << it->file << it->md5 << it->crc;
    return ret;
}

void Dtls_Recver::rcvClientMessage(const QByteArray &data)
{
    if(mFile->isWritable()) {
        if(mSize) mFile->write(data);
        else initFile(data);
        mSize += data.size();
    } else throwMessage(tr("Error: Dtls Recver write file %1").arg(data.data()));
}
