#include "op_updater.h"

OP_Updater::OP_Updater(QObject *parent) : OP_Object{parent}
{
    isOta = false;
}

bool OP_Updater::ota_update(int addr, const QByteArray &array)
{
    bool ret = initOta(addr);
    if(ret) {
        QByteArray data; int max = 1024;
        for(int i=0; i<array.size();i+=data.size()) {
            data = array.mid(i, max);
            ret = sendPacket(addr, data);
            if(ret) mdelay(20); else break;
        } isOta = false;
    }

    return ret;
}

bool OP_Updater::ota_update(int addr, const QString &fn)
{
    QFile file(fn);  bool ret = false; int max = 1024;
    if(file.exists() && file.open(QIODevice::ReadOnly)) {
        ret = initOta(addr);
        while (!file.atEnd() && ret) {
            QByteArray data = file.read(max);
            ret = sendPacket(addr, data);
            if(ret) mdelay(20); else break;
        } file.close(); isOta = false;
    }

    return ret;
}

bool OP_Updater::initOta(int id)
{
    isOta = true; waitForLock(); mdelay(200);
    uchar cmd[] = {0x7B, 0xA5, 0x01, 0x10, 0x00, 0x00,
                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                   0x00, 0x00, 0x00, 0xCF};
    cmd[2] = id; cmd[15] = xorNum(cmd,sizeof(cmd)-1);
    QByteArray recv = transmit(cmd, sizeof(cmd), 2000); emit otaSig(recv);
    if(!recv.contains("Start Updata")) isOta = false;
    return isOta;
}

bool OP_Updater::sendPacket(int addr, const QByteArray &array)
{
    bool ret = true;
    QByteArray data;
    data.append(0x7B);
    data.append(addr);
    data.append(array.size() / 256);
    data.append(array.size() % 256);
    data.append(array);

    uchar x = xorNum(data); data.append(x);
    QByteArray recv = transmit(data, 2000);  emit otaSig(recv);
    if(recv.contains("Receive Packet Failure")) ret = false;
    return ret;
}
