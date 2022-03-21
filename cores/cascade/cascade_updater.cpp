#include "cascade_updater.h"

Cascade_Updater::Cascade_Updater(QObject *parent) : Cascade_Object{parent}
{
    isOta = false;
    mFile = new QFile;
}

bool Cascade_Updater::ota_update(int addr, const QString &fn)
{
    bool ret = false; int max = 4*1024;
    mFile->close(); mFile->setFileName(fn);
    if(mFile->exists() && mFile->open(QIODevice::ReadOnly)) {
        ret = otaSendInit(addr, fn);
        while (!mFile->atEnd() && ret) {
            QByteArray data = mFile->read(max);
            ret = otaSendPacket(addr, data);
            if(ret) cm::mdelay(25); else break;
        } mFile->close(); otaSendFinish(addr, ret?1:0); isOta = false;
    }

    return ret;
}

void Cascade_Updater::ota_updates(const QString &fn)
{
    sDevData *dev = cm::masterDev();
    for(int i=0; i<dev->info.slaveNum; ++i) {
        ota_update(i+1, fn);
    }
}

void Cascade_Updater::ota_updates()
{
    if(mOtaFile.size()) {
        ota_updates(mOtaFile);
    } mOtaFile.clear();
}

bool Cascade_Updater::otaSendInit(int addr, const QString &fn)
{
    isOta = true; waitForLock(); cm::mdelay(200);
    QByteArray recv = transData(fc_otaStart, addr, fn.toLocal8Bit());
    if(!recv.contains("Start Updata")) isOta = false;
    return isOta;
}

bool Cascade_Updater::otaSendFinish(int addr, uint state)
{
    return otaSendData(fc_otaEnd, addr, QByteArray::number(state));
}

bool Cascade_Updater::otaSendPacket( int addr, const QByteArray &array)
{
    return otaSendData(fc_otaPack, addr, array);
}

bool Cascade_Updater::otaSendData(uchar fn, int addr, const QByteArray &array)
{
    bool ret = true;
    QByteArray recv = transData(fn, addr, array);  emit otaSendSig(addr, recv);
    if(recv.isEmpty() || recv.contains("Receive Packet Failure")) ret = false;
    return ret;
}


bool Cascade_Updater::otaSetFile(const QString &fn)
{
    mFile->close();
    mFile->setFileName(fn);
    bool ret = mFile->open(QIODevice::WriteOnly | QIODevice::Truncate);
    if(ret) mSize = 0; else qDebug() << tr("Error: Cascade Recver open file").arg(fn);
    return ret;
}

bool Cascade_Updater::otaReplyStart(const QByteArray &data)
{
    otaSetFile(data);
    return writeData(fc_otaStart, 0, "Start Updata");
}

bool Cascade_Updater::otaReplyPacket(const QByteArray &data)
{
    bool ret = false;
    if(mFile->isWritable()) {
        mFile->write(data);
        mSize += data.size();
        QString str = "Receive Packet " + QString::number(mSize);
        ret = writeData(fc_otaPack, 0, str.toLocal8Bit());
    } else qDebug() << tr("Error: reply Ota Packet %1").arg(data.data());

    return ret;
}

bool Cascade_Updater::otaReplyFinish(const QByteArray &data)
{    
    QString str = "Receive Packet "; mFile->close();
    bool ret = data.toInt(); emit otaReplyFinishSig(ret);
    if(ret) str += QString::number(mSize); else str += "Failure";
    return writeData(fc_otaEnd, 0, str.toLocal8Bit());
}
