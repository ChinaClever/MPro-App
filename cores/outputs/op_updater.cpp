/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "op_updater.h"


OP_Updater::OP_Updater(QObject *parent) : OP_Object{parent}
{
    isOta = false;
}

bool OP_Updater::ota_start(const QString &fn)
{
    bool ret = File::CheckCrc(fn); if(ret) mOtaFile=fn;
    else qDebug() << "Error: OP Updater ota crc" << Q_FUNC_INFO;
    return ret;
}

bool OP_Updater::ota_updates()
{
    bool ret = false;
    if(mOtaFile.size() > 0) {
        QString fn = mOtaFile; mOtaFile.clear();
        for(uint i=0; i< mDev->cfg.nums.boardNum; ++i) {
            ret = ota_update(i+1, fn);
            emit otaFinish(i+1, ret);
        } cm::mdelay(220);
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
            if(ret) cm::mdelay(220); else break;
        }
    }

    return ret;
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
            if(ret) cm::mdelay(220); else break;
        } file.close(); isOta = false;
    }

    return ret;
}

bool OP_Updater::initOta(int id)
{
    isOta = true; waitForLock(); cm::mdelay(200);
    uchar cmd[] = {0x7B, 0xA5, 0x01, 0x10, 0x00, 0x00,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                   0x00, 0x00, 0x00, 0xCF};
    cmd[2] = id; cmd[15] = Crc::XorNum(cmd,sizeof(cmd)-1);
    QByteArray recv = transmit(cmd, sizeof(cmd), 5000);
    if(!recv.contains("Start Updat")) isOta = false;
    emit otaSig(id, recv);
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
    Crc::AppendCrc(data); QByteArray recv = transmit(data, 3000);
    if(recv.contains("success")) ret = true;
    emit otaSig(addr, recv);
    return ret;
}
