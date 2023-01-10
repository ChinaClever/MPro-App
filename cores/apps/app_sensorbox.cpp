/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "app_sensorbox.h"
//#include "crc.h"

App_SensorBox::App_SensorBox(QObject *parent)
    : App_Sensor{parent}
{
    QtConcurrent::run(this, &App_SensorBox::sensorBox_run);
}

App_SensorBox::~App_SensorBox()
{
    box_isRun = false;
}

bool App_SensorBox::box_open()
{
    bool ret = cm::masterDev()->cfg.param.sensorBoxEn;
    if(ret) {
        qint32 baudRate = QSerialPort::Baud19200;
        if(!mSerial) mSerial = new SerialPort(this);
        ret = mSerial->isOpened();
        if(!ret) ret = mSerial->openSerial("/dev/ttyS5", baudRate);
    }
    return ret;
}

void App_SensorBox::box_close()
{
    bool ret = cm::masterDev()->cfg.param.sensorBoxEn;
    if(!ret) if(mSerial) mSerial->closeSerial();
}

ushort App_SensorBox::getShort(uchar *ptr)
{
    return  (*ptr) * 256 + *(ptr+1);
}

bool App_SensorBox::box_recvPacket(const QByteArray &array)
{
    bool ret = true;
    uchar *ptr = (uchar *)array.data();
    sEnvData *env = &(cm::masterDev()->env);

    if((*ptr++ == 0x01) && (*ptr++ == 0x03))  {
        int len = getShort(ptr); ptr += 2;
        ushort isInsert = getShort(ptr); ptr += 2;
        env->tem.value[2] = getShort(ptr); ptr += 2;
        env->hum.value[2] = getShort(ptr); ptr += 2;
        env->tem.value[3] = getShort(ptr); ptr += 2;
        env->hum.value[3] = getShort(ptr); ptr += 2;
        ptr += 8; int k=0, j=4; // 报警上下限
        ushort alarm = getShort(ptr); //ptr += 2;

        env->isInsert[2] = (isInsert >> k++) & 1;
        env->isInsert[3] = (isInsert >> k++) & 1;
        env->smoke[0] = (isInsert >> k++) & 1;
        env->water[0] = (isInsert >> k++) & 1;
        env->door[0] = (isInsert >> k++) & 1;
        env->door[1] = (isInsert >> k++) & 1;

        if(env->smoke[0]) env->smoke[0] += (alarm >> j++) & 1;
        if(env->water[0]) env->water[0] += (alarm >> j++) & 1;
        if(env->door[0]) env->door[0] += (alarm >> j++) & 1;
        if(env->door[1]) env->door[1] += (alarm >> j++) & 1;
    } else ret = false;

    return ret;
}

bool App_SensorBox::box_readData()
{
    bool res = false;
    uchar cmd[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x0B, 0x04, 0x0D};
    QByteArray recv = mSerial->transmit(cmd, sizeof(cmd));
    if(recv.size() > 20) res = box_recvPacket(recv);

    int t = 0; if(cm::runTime() > 48*60*60) {
        t = QRandomGenerator::global()->bounded(565);
    } cm::mdelay(t + 360);
    return res;
}

void App_SensorBox::sensorBox_run()
{
    while(box_isRun) {
        if(cm::masterDev()->cfg.param.sensorBoxEn) {
            box_open();
            box_readData();
        } else {
            box_close();
            cm::mdelay(360);
        }
    }
}
