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
    QString str = "cascade updater " + msg;
    QString ip = cm::dataPacket()->ota.host; qDebug() << str;
    if(ip.size()) mNet->writeDatagram(str.toUtf8(), QHostAddress(ip), 21437);
}

bool Cascade_Updater::ota_update(int addr, const sOtaFile &it)
{
    sOtaUpIt *up = &(cm::dataPacket()->ota.slave);
    bool ret = false; int max = 3*1024; int i=0, pro=0;
    mFile->close(); mFile->setFileName(it.path + it.file);
    if(mFile->exists() && mFile->open(QIODevice::ReadOnly)) {
        ret = otaSendInit(addr, it); cm::mdelay(150);
        while (!mFile->atEnd() && ret) {
            QByteArray data = mFile->read(max);
            ret = otaSendPacket(addr, data);
            if(ret) {
                i += data.size(); int v = (i*100.0)/it.size;
                if(v > pro){ pro = up->progs[addr] = v; up->results[addr] = 1;
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
        sOtaUpIt *up = &(cm::dataPacket()->ota.slave);
        if(size) setbit(cm::dataPacket()->ota.work, DOta_Slave);
        for(int i=0; i<DEV_NUM; ++i) up->progs[i] = up->results[i] = 0;
        for(uint i=0; i<size; ++i) {
            throwMessage(tr("addr=%1: %2").arg(i+1).arg(0));
            if(cm::devData(i+1)->offLine) ota_update(i+1, mIt);
        } mIt.file.clear(); clrbit(cm::dataPacket()->ota.work, DOta_Slave);
        if(cm::dataPacket()->ota.work) isOta = false; else otaReboot();
        up->isRun = 0; cm::mdelay(255);
    }
}

bool Cascade_Updater::otaSendInit(int addr, const sOtaFile &it)
{
    isOta = true; waitForLock(); cm::mdelay(200); mCnt = 0;
    QByteArray array; QDataStream in(&array, QIODevice::WriteOnly);
    in << it.fc << it.dev <<it.path << it.file << it.md5 << it.sig << it.size << END_CRC;
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
    bool ret = true; cm::mdelay(165); mCnt++;
    QByteArray recv = tranData(fn, addr, array);
    if(recv.isEmpty() || recv.contains("Receive Packet Failure")) {
        throwMessage(tr("cascade error: addr=%1: fn=%2 cnt=%3 ota send len:%4 recv failed: %5")
                     .arg(addr).arg(fn).arg(mCnt).arg(array.size()).arg(recv.data())); ret = false;
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
    out >> it->fc >> it->dev >> it->path >> it->file >> it->md5 >> it->sig >> it->size >> it->crc;
    if(it->crc == END_CRC) otaSetFile(it->path + it->file);
    else cout << it->file << it->md5 << it->sig << it->crc;
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
        QString fn = it.path + it.file;
        QString dir = "/tmp/updater/ota_apps/";
        QString str = "unzip -o %1 -d " + dir;
        qDebug() << cm::execute(str.arg(fn));
        system("chmod 777 -R /usr/data/clever/");
        system("chmod 777 -R /tmp/updater/ota_apps/");
        QString fmd = "rsync -av --exclude rootfs/  %1 /usr/data/clever/";
        QString cmd = fmd.arg(dir); throwMessage(cmd);
        str = cm::execute(cmd); throwMessage(str);
        Set_Core::bulid()->ota_updater(11, fn);
        otaRootfs(dir);

        cm::dataPacket()->ota.slave.isRun = 0;
        clrbit(cm::dataPacket()->ota.work, DOta_Slave);
        if(cm::dataPacket()->ota.work) isOta = false; else otaReboot();
        //if(!ret) QTimer::singleShot(3555,this,SLOT(rebootSlot()));
    } else {
        cm::dataPacket()->ota.slave.isRun = 2;
        QString cmd = "rm -rf " + it.path + it.file;
        system(cmd.toUtf8().data());
    }
}

void Cascade_Updater::otaRootfs(const QString &path)
{
    QString dir = path + "rootfs";
    QStringList fns = File::entryList(dir); cout << dir << fns;
    if(fns.contains("rootfs.squashfs") && fns.contains("xImage")) {
        QString fmd = "system-update %1/xImage %1/rootfs.squashfs";
        sOtaUpdater *ota = &cm::dataPacket()->ota;
        setbit(ota->work, DOta_Rootfs);
        throwMessage(fmd.arg(dir));
        ota->rootfs.progress = 0;
        ota->rootfs.isRun = 1;
        QString str = fmd.arg(dir);
        system(str.toLocal8Bit().data());
        throwMessage(str);
        ota->rootfs.isRun = 0;
        ota->rootfs.progress = 100;
        clrbit(cm::dataPacket()->ota.work, DOta_Rootfs);
    }
}

void Cascade_Updater::otaReboot()
{
    system("rm -rf /usr/data/clever/outlet/*");
    system("chmod 777 /usr/data/clever/bin/*");
    system("chmod 777 /usr/data/clever/app/*");
    system("rm -rf /tmp/updater/ota_apps");
    system("rm -rf /usr/data/upload/*");
    system("sync"); system("reboot");
}
