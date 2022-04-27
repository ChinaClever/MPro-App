#include "ymodem.h"
#include "commons.h"

YModem::YModem(QObject *parent)
    : QObject{parent},
      mTransmit(new YmodemFileTransmit(parent)),
      mReceive(new YmodemFileReceive(parent))
{

    connect(mReceive, SIGNAL(receiveProgress(int)), this, SIGNAL(progressSig(int)));
    connect(mTransmit, SIGNAL(transmitProgress(int)), this, SIGNAL(progressSig(int)));

    connect(mTransmit, SIGNAL(transmitStatus(Ymodem::Status)), this, SLOT(upStatus(Ymodem::Status)));
    connect(mTransmit, SIGNAL(transmitStatus(Ymodem::Status)), this, SIGNAL(statusSig(Ymodem::Status)));

    connect(mReceive, SIGNAL(receiveStatus(Ymodem::Status)), this, SLOT(upStatus(Ymodem::Status)));
    connect(mReceive, SIGNAL(receiveStatus(Ymodem::Status)), this, SIGNAL(statusSig(Ymodem::Status)));
}

bool YModem::waitForFinish(int min)
{
    for(int i=0; i< min*60*1000; i+=200) {
        uchar state = mStatus; cm::mdelay(100);
        if(state == YmodemFileReceive::StatusFinish) return true;
        else if(state > YmodemFileReceive::StatusFinish) return false;
        else cm::mdelay(100);
    }

    return false;
}

void YModem::upStatus(YmodemFileReceive::Status status)
{
    mStatus = status;
}

bool YModem::startTransmit(const QString &name, QSerialPort *serial)
{
    mStatus = 0;
    mTransmit->setFileName(name);
    mTransmit->initSerialPort(serial);
    bool ret = mTransmit->startTransmit();
    if(ret) ret = waitForFinish();
    return ret;
}

bool YModem::startReceive(const QString &path, QSerialPort *serial)
{
    mStatus = 0;
    mReceive->setFilePath(path);
    mReceive->initSerialPort(serial);
    bool ret = mReceive->startReceive();
    if(ret) ret = waitForFinish();
    return ret;
}

void YModem::stop()
{
    mReceive->stopReceive();
    mTransmit->stopTransmit();
}
