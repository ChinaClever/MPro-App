/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "cascade_updater.h"

Cascade_Updater::Cascade_Updater(QObject *parent) : Cascade_Object{parent}
{
    mNet = new Net_Udp(this);
    isOta = false; mFile = new QFile;
    qRegisterMetaType<sOtaFile>("sOtaFile");
    connect(this, &Cascade_Updater::otaReplyFinishSig,
            this, &Cascade_Updater::otaRecvFinishSlot);
}

void Cascade_Updater::throwMessage(const QString &msg)
{
    QString str = "updater cascade " + msg;
    QString ip = cm::dataPacket()->ota.host;
    if(ip.size()) mNet->writeDatagram(str.toUtf8(), QHostAddress(ip), 21437);
}

bool Cascade_Updater::ota_update(int addr, const sOtaFile &it)
{
    sOtaUpIt *up = &(cm::dataPacket()->ota.slave);
    bool ret = false; int max = 10*1024; int i=0, pro=0;
    mFile->close(); mFile->setFileName(it.path + it.file);
    if(mFile->exists() && mFile->open(QIODevice::ReadOnly)) {
        ret = otaSendInit(addr, it); cm::mdelay(5);
        while (!mFile->atEnd() && ret) {
            QByteArray data = mFile->read(max);
            ret = otaSendPacket(addr, data);
            if(ret) {
                i += data.size(); int v = (i*100.0)/it.size;
                if(v > pro){ pro = v; up->results[addr] = 1;
                    throwMessage(tr("addr=%1: %2").arg(addr).arg(v));
                    up->subId = addr; up->progress = v; up->isRun = 1;                    
                }
            } else {
                up->isRun = 2; up->results[addr] = 3;
                throwMessage(tr("Error: addr=%1: ota update failed").arg(addr)); break;
            }
        } mFile->close(); ret = otaSendFinish(addr, ret?1:0);
    } cm::mdelay(1200); if(ret) up->results[addr] = 2;

    return ret;
}


void Cascade_Updater::ota_updates()
{
    if(mIt.file.size()) {
        sDevData *dev = cm::masterDev();
        uint size = dev->cfg.nums.slaveNum;
        if(size) setbit(cm::dataPacket()->ota.work, DOta_Slave);
        for(uint i=0; i<size; ++i) {
            if(cm::devData(i+1)->offLine) ota_update(i+1, mIt);
        } mIt.file.clear(); clrbit(cm::dataPacket()->ota.work, DOta_Slave);
        if(cm::dataPacket()->ota.work) isOta = false; else otaReboot();
        cm::dataPacket()->ota.slave.isRun = 0; cm::mdelay(255);
    }
}

bool Cascade_Updater::otaSendInit(int addr, const sOtaFile &it)
{
    isOta = true; waitForLock(); cm::mdelay(200);
    QByteArray array; QDataStream in(&array, QIODevice::WriteOnly);
    in << it.fc << it.dev <<it.path << it.file << it.md5 << it.size << END_CRC;
    QByteArray recv = tranData(fc_otaStart, addr, array);
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
    QByteArray recv = tranData(fn, addr, array); cm::mdelay(5);
    if(recv.isEmpty() || recv.contains("Receive Packet Failure")) {
        throwMessage(tr("Error: addr=%1: ota send data %2 recv failed: %3")
                     .arg(addr).arg(fn).arg(recv.data())); ret = false;
    } //emit otaSendSig(addr, recv);
    return ret;
}

bool Cascade_Updater::otaSetFile(const QString &fn)
{
    mFile->close(); mFile->setFileName(fn);
    bool ret = mFile->open(QIODevice::WriteOnly | QIODevice::Truncate);
    if(ret) mSize = 0; else cout << fn;
    return ret;
}

bool Cascade_Updater::otaReplyStart(const QByteArray &data)
{
    sOtaFile *it = &mIt; QByteArray rcv(data); cm::dataPacket()->ota.slave.isRun = 1;
    QDataStream out(&rcv, QIODevice::ReadOnly); setbit(cm::dataPacket()->ota.work, DOta_Slave);
    out >> it->fc >> it->dev >> it->path >> it->file >> it->md5 >> it->size >> it->crc;
    if(it->crc == END_CRC) otaSetFile(it->path + it->file);
    else cout << it->file << it->md5 << it->crc;
    return writeData(fc_otaStart, 0, "Start Updata");
}

bool Cascade_Updater::otaReplyPacket(const QByteArray &data)
{
    bool ret = false;
    if(mFile->isWritable()) {
        mFile->write(data); mSize += data.size();
        QString str = "Receive Packet " + QString::number(mSize);
        ret = writeData(fc_otaPack, 0, str.toLocal8Bit());
    } else cout << tr("Error: reply Ota Packet %1").arg(data.data());

    return ret;
}

bool Cascade_Updater::otaReplyFinish(const QByteArray &data)
{    
    QString str = "Receive Packet "; mFile->close();
    bool ret = data.toInt() && File::CheckMd5(mIt);  emit otaReplyFinishSig(mIt, ret);
    if(ret) str += QString::number(mSize) + " successful"; else str += "Failure";
    return writeData(fc_otaEnd, 0, str.toLocal8Bit());
}

void Cascade_Updater::otaRecvFinishSlot(const sOtaFile &it, bool ok)
{
    if(ok){
        QString dst = "/usr/data/", fn = it.path + it.file;
        QString str = "unzip -o %1 -d " + dst + "updater/clever/";
        qDebug() << cm::execute(str.arg(fn));
        cm::dataPacket()->ota.slave.isRun = 0;
        clrbit(cm::dataPacket()->ota.work, DOta_Slave);
        bool ret = Set_Core::bulid()->ota_updater(11, fn);
        if(!ret) QTimer::singleShot(3555,this,SLOT(rebootSlot()));
    } else {
        cm::dataPacket()->ota.slave.isRun = 2;
        QString cmd = "rm -rf " + it.path + it.file;
        system(cmd.toUtf8().data());
    }
}

void Cascade_Updater::otaReboot()
{
    QString cmd = "cp -af /usr/data/updater/clever/  /usr/data/";
    throwMessage(cm::execute(cmd));

    system("chmod +x /usr/data/clever/bin/*");
    system("chmod +x /usr/data/clever/app/*");
    cmd = "rm -rf /usr/data/updater/clever";
    throwMessage(cm::execute(cmd));
    cm::execute("rm -rf /usr/data/clever/upload/*");
    cm::execute("sync"); system("reboot");
}
