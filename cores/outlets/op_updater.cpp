/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "op_updater.h"


OP_Updater::OP_Updater(QObject *parent) : OP_Object{parent}
{    
    mNet = new Net_Udp(this); isOta = false;
    connect(this, &OP_Updater::otaSig, this, &OP_Updater::onOtaSig);
    connect(this, &OP_Updater::otaFinish, this, &OP_Updater::onOtaFinish);
    connect(this, &OP_Updater::otaProgress, this, &OP_Updater::onOtaProgress);
}

bool OP_Updater::ota_start(const QString &fn)
{
    bool ret = true; // File::CheckCrc(fn); //////////==========
    if(ret) { mOtaFile=fn; throwMessage("start");
        setbit(cm::dataPacket()->ota.work, DOta_Outlet);
    } else qDebug() << "Error: OP Updater ota crc" << Q_FUNC_INFO;
    return ret;
}

void OP_Updater::throwMessage(const QString &msg)
{
    QString str = "ota updater outlet " + msg;
    QString ip = cm::dataPacket()->ota.host;  qDebug() << str;
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
    system("chmod 777 /usr/data/pdu/bin/*");
    system("chmod 777 /usr/data/pdu/app/*");
    system("rm -rf /usr/data/pdu/outlet/*");
    system("rm -rf /tmp/updater/ota_apps");
    system("rm -rf /usr/data/upload/*");

    QString dir = "/tmp/mass_storage/sda/ota_apps/ver.ini";
    QString dir1 = "/tmp/mass_storage/sda1/ota_apps/ver.ini";
    QString dir2 = "/tmp/mass_storage/sda2/ota_apps/ver.ini";
    while(QFile::exists(dir) || QFile::exists(dir1) || QFile::exists(dir2)) cm::mdelay(1000);
    system("sync"); system("reboot");
}

bool OP_Updater::ota_updates()
{
    bool ret = false;
    if(mOtaFile.size() > 0) {
        QString fn = mOtaFile; mOtaFile.clear();
        sOtaUpIt *up = &cm::dataPacket()->ota.outlet; up->isRun = 1; /*执行版升级状态置为正在进行中*/
        for(int i=0; i<DEV_NUM; ++i) up->progs[i] = up->results[i] = 0; /*初始化升级结果和升级状态*/
        if(mDev->cfg.param.devSpec == 1) ota_updateA(up, fn);
        else for(uint i=1; i<=mDev->cfg.nums.boardNum; ++i) { /*执行版数量*/
            up->results[i] = 1; for(int k=0; k<3; ++k) {
                ret = ota_update(i, fn); if(ret) break; else cm::mdelay(1200);  /*ota升级，成功返回true*/
            } if(ret) up->results[i] = 2; else up->results[i] = 3;  /*返回值为fasle，升级结果为失败，否则为成功*/
            emit otaFinish(i, ret); cm::mdelay(5*1200);
        } cm::mdelay(220); isOta = false; up->isRun = ret?0:2;
        clrbit(cm::dataPacket()->ota.work, DOta_Outlet);
        if(ret) system("rm -rf /usr/data/pdu/outlet/*");
        if(!cm::dataPacket()->ota.work) ota_reboot();
    }

    return ret;
}

bool OP_Updater::ota_updateA(sOtaUpIt *up, const QString &fn)
{
    bool ret = false; up->results[1] = 1; ret = ota_update(4, fn);
    if(ret) up->results[1] = 2; else up->results[1] = 3;
    emit otaFinish(4, ret); cm::mdelay(4*1200);
    return ret;
}

#if 0
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
#endif

void OP_Updater::onOtaProgress(uchar addr, int v)
{
    QString str = "addr=%1 progress=%2%";
    throwMessage(str.arg(addr).arg(v/10.0));
    if(1 == mDev->cfg.param.devSpec) addr = 1;
    sOtaUpIt *it = &cm::dataPacket()->ota.outlet;
    it->subId = addr; it->progress = it->progs[addr] = v/10;    
    it->progs[DEV_NUM/2+addr] = v%10;
}

bool OP_Updater::ota_update(int addr, const QString &fn)
{
    QFile file(fn);  bool ret = false; int max = 1024;
    if(file.exists() && file.open(QIODevice::ReadOnly)) {       
        ret = initOta(addr); uint size = file.size(), len=0;
        file.seek(0); while (!file.atEnd() && ret) {
            QByteArray data = file.read(max);
            ret = sendPacket(addr, data); len += data.size();
            int v = (len*1000.0)/size; emit otaProgress(addr, v);
            if(ret) cm::mdelay(325); else break;
        } file.close(); if(len+1024 < size) ret = false;
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
    QByteArray recv = transmit(cmd, sizeof(cmd), 4000);
    if(!recv.contains("Start Updat")) { cm::mdelay(3250);
        recv = transmit(cmd, sizeof(cmd), 5500);
        if(!recv.isEmpty()) isOta = false;
        //if(!recv.contains("Start Updat")) isOta = false;
    } emit otaSig(id, recv);
    return isOta;
}

bool OP_Updater::sendPacket(int addr, const QByteArray &array)
{
    bool ret = false; QByteArray data; data.append(0x7B);
    if(array.size()==1024) data.append(addr);
    else data.append(0xC0 +addr);

    data.append(array.size() / 256);
    data.append(array.size() % 256);
    data.append(array);

    for(int i=array.size(); i<1024; ++i) data.append((char)0);
    Crc::RtuAppend(data); QByteArray recv = transmit(data, 3000);
    if(recv.contains("success")) ret = true; else cout << recv.size() << recv;
    emit otaSig(addr, recv); //if(1 == mDev->cfg.param.devSpec) ret = true;
    return ret;
}
