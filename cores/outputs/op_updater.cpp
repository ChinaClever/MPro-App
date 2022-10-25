/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "op_updater.h"


OP_Updater::OP_Updater(QObject *parent) : OP_Object{parent}
{
    isOta = false;
    mNet = new Net_Udp(this);
    connect(this, &OP_Updater::otaSig, this, &OP_Updater::onOtaSig);
    connect(this, &OP_Updater::otaFinish, this, &OP_Updater::onOtaFinish);
    connect(this, &OP_Updater::otaProgress, this, &OP_Updater::onOtaProgress);
}

bool OP_Updater::ota_start(const QString &fn)
{
    bool ret = true; // File::CheckCrc(fn); //////////==========
    if(ret) { mOtaFile=fn;
        setbit(cm::dataPacket()->ota.work, 4);
    } else qDebug() << "Error: OP Updater ota crc" << Q_FUNC_INFO;
    return ret;
}

void OP_Updater::throwMessage(const QString &msg)
{
    QString str = "updater outlet " + msg;
    QString ip = cm::dataPacket()->ota.host;  //qDebug() << str;
    if(ip.size()) mNet->writeDatagram(str.toUtf8(), QHostAddress(ip), 21437);
}

void OP_Updater::onOtaFinish(uchar addr, bool ok)
{
    QString str = "addr=%1 upgrade results ";
    if(ok) str += "ok"; else str += "err";
    throwMessage(str.arg(addr));
}

void OP_Updater::ota_reboot()
{
    QString cmd = "cp -af /usr/data/updater/clever/  /usr/data/";
    throwMessage(cm::execute(cmd));
    system("chmod +x /usr/data/clever/bin/*");
    system("chmod +x /usr/data/clever/app/*");
    cm::execute("rm -rf /usr/data/clever/outlet/*");
    cmd = "rm -rf /usr/data/updater/clever";
    throwMessage(cm::execute(cmd));

    cm::execute("rm -rf /usr/data/clever/upload/*");
    cm::execute("sync"); system("reboot");
}

bool OP_Updater::ota_updates()
{
    bool ret = false;
    if(mOtaFile.size() > 0) {
        QString fn = mOtaFile; mOtaFile.clear();
        cm::dataPacket()->ota.outlet.isRun = 1;
        for(uint i=0; i< mDev->cfg.nums.boardNum; ++i) {
            ret = ota_update(i+1, fn);
            emit otaFinish(i+1, ret);
        } cm::mdelay(220); isOta = false;
        cm::dataPacket()->ota.outlet.isRun = ret?0:2;
        clrbit(cm::dataPacket()->ota.work, 4);
        if(ret) system("rm -rf /usr/data/updater/clever/outlet/*");
        if(!cm::dataPacket()->ota.work) ota_reboot();
    }

    return ret;
}

bool OP_Updater::ota_update(int addr, QByteArray &array)
{
    bool ret = initOta(addr);
    if(ret) {
        QByteArray data; int max = 1024;
        for(int i=0; i<array.size();i+=data.size()) {
            data = array.mid(i, max);
            ret = sendPacket(addr, data);
            if(ret) cm::mdelay(225); else break;
        }
    }

    return ret;
}

void OP_Updater::onOtaProgress(uchar addr, int v)
{
    sOtaUpIt *it = &cm::dataPacket()->ota.outlet;
    it->subId = addr; it->progress = v;
    QString str = "addr=%1 progress=%2";
    throwMessage(str.arg(addr).arg(v));
}

bool OP_Updater::ota_update(int addr, const QString &fn)
{
    QFile file(fn);  bool ret = false; int max = 1024;
    if(file.exists() && file.open(QIODevice::ReadOnly)) {
        ret = initOta(addr); uint size = file.size(), len=0;
        while (!file.atEnd() && ret) {
            QByteArray data = file.read(max);
            ret = sendPacket(addr, data); len += data.size();
            int v = (len*100.0)/size; emit otaProgress(addr, v);
            if(ret) cm::mdelay(325); else break;
        } file.close();
    } else cout << addr << fn;

    return ret;
}

void OP_Updater::onOtaSig(int addr, const QString &msg)
{
    QString str = "addr=%1 recv=%2";
    throwMessage(str.arg(addr).arg(msg));
}

bool OP_Updater::initOta(int id)
{
    isOta = true; waitForLock(); cm::mdelay(600);
    uchar cmd[] = {0x7B, 0xA5, 0x01, 0x10, 0x00, 0x00,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                   0x00, 0x00, 0x00, 0xCF};
    cmd[2] = id; cmd[15] = Crc::XorNum(cmd,sizeof(cmd)-1);
    QByteArray recv = transmit(cmd, sizeof(cmd), 11*1000);
    if(!recv.contains("Start Updat")) {
        recv = transmit(cmd, sizeof(cmd), 15*1000);
        if(!recv.contains("Start Updat")) isOta = false;
    } emit otaSig(id, recv);
    return isOta;
}

bool OP_Updater::sendPacket(int addr, const QByteArray &array)
{
    bool ret = false; QByteArray data;
    data.append(0x7B); data.append(addr);
    data.append(array.size() / 256);
    data.append(array.size() % 256);
    data.append(array);

    for(int i=array.size(); i<1024; ++i) data.append((char)0);
    Crc::AppendCrc(data); QByteArray recv = transmit(data, 11*1000);
    if(recv.contains("success")) ret = true;
    emit otaSig(addr, recv);
    return ret;
}
