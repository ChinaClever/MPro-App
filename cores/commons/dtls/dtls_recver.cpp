/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "dtls_recver.h"


Dtls_Recver::Dtls_Recver(QObject *parent)
    : QObject{parent}
{
    mFile = new QFile;
    mDtls = new Dtls_Service(this); isFinshed = false;
    mNet = new Net_Udp(this); mTimer = new QTimer(this);
    connect(mDtls, &Dtls_Service::errorMessage, this, &Dtls_Recver::throwError);
    connect(mDtls, &Dtls_Service::warningMessage, this, &Dtls_Recver::throwMessage);
    connect(mDtls, &Dtls_Service::infoMessage, this, &Dtls_Recver::throwMessage);
    connect(mDtls, &Dtls_Service::datagramReceived, this, &Dtls_Recver::rcvClientMessage);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(onTimeoutDone()));
}


Dtls_Recver *Dtls_Recver::bulid(QObject *parent)
{
    static Dtls_Recver* sington = nullptr;
    if(sington == nullptr) {
        sington = new Dtls_Recver(parent);
    }
    return sington;
}

bool Dtls_Recver::setFile(const QString &fn)
{    
    system("mkdir -p /usr/data/upload");
    mFile->setFileName(fn); isFinshed = false;
    bool ret = mFile->open(QIODevice::WriteOnly | QIODevice::Truncate);
    if(ret) mSize = 0; else throwMessage(tr("Error: Dtls Recver open file %1").arg(fn));
    emit startSig(mDtls->clientHost().toString().remove("::ffff:"));
    return ret;
}

bool Dtls_Recver::waitForFinish()
{
    while(!isFinshed) cm::mdelay(2);
    return recvFinish();
}

void Dtls_Recver::onTimeoutDone()
{
    if(mSize > mCnt) mCnt = mSize;
    else if(!isFinshed) throwError(tr("传输中断"));
}

void Dtls_Recver::throwError(const QString &message)
{
    bool ret = recvFinish();
    if(ret) throwMessage(tr("%1 接收成功").arg(mDtls->clientHost().toString()));
    else {throwMessage(message);} emit finishSig(mIt, ret);
}

void Dtls_Recver::throwMessage(const QString &message)
{
    emit messageSig(message); // qDebug() << message;
    mNet->writeDatagram(message.toUtf8(), mDtls->clientHost(), 21437);
}

bool Dtls_Recver::recvFinish()
{
    isFinshed = true;
    mTimer->stop(); mSize = 0;
    if(mFile->isOpen()) mFile->close();
    return File::CheckMd5(mIt);
}

bool Dtls_Recver::initFile(const QByteArray &array)
{
    QByteArray rcv(array); bool ret = false; mTimer->start(1000);
    sOtaFile *it = &mIt; QDataStream out(&rcv, QIODevice::ReadOnly); mCnt = 0;
    out >> it->fc >> it->dev >> it->path >> it->file >> it->md5 >> it->sig >> it->size >> it->crc;
    if(it->crc == END_CRC) ret = setFile(it->path + it->file);
    else throwMessage("Error: Dtls recver head");
    return ret;
}

void Dtls_Recver::rcvClientMessage(const QByteArray &data)
{
    if(mFile->isWritable() && mSize) mFile->write(data);
    else if(!mFile->isOpen() && !mSize) initFile(data);
    else throwMessage(tr("Error: Dtls Recver write file %1").arg(data.size()));
    mSize += data.size(); //throwMessage(tr("Dtls Recver File Size %1").arg(mSize));
}
