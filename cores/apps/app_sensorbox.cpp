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
    bool ret = false;
    if(cm::masterDev()->cfg.param.sensorBoxEn) {
        qint32 baudRate = QSerialPort::Baud9600;
        if(!mSerial) mSerial = new SerialPort(this);
        ret = mSerial->isOpened();
        if(!ret) ret = mSerial->openSerial("/dev/ttyS4", baudRate);
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
        int len = getShort(ptr); //cout << len;
        if(len == 22) ptr += 2; else return false;
        ushort isInsert = getShort(ptr); ptr += 2;
        env->tem.value[2] = getShort(ptr); ptr += 2;
        env->hum.value[2] = getShort(ptr); ptr += 2;
        env->tem.value[3] = getShort(ptr); ptr += 2;
        env->hum.value[3] = getShort(ptr); ptr += 2;
        ptr += 8; int k=0, j=4;// 报警上下限
        ushort alarm = getShort(ptr); //ptr += 2;

        env->isInsert[2] = (isInsert >> k++) & 1;
        env->isInsert[3] = (isInsert >> k++) & 1;
        env->smoke[0] = (isInsert >> k++) & 1;
        env->water[0] = (isInsert >> k++) & 1;
        //env->door[0] = (isInsert >> k++) & 1;
        //env->door[1] = (isInsert >> k++) & 1;

        if(env->tem.value[2]) env->tem.value[2] += 400;
        if(env->tem.value[3]) env->tem.value[3] += 400;
        if(env->smoke[0]) env->smoke[0] += (alarm >> j++) & 1;
        if(env->water[0]) env->water[0] += (alarm >> j++) & 1;
        //if(env->door[0]) env->door[0] += (alarm >> j++) & 1;
        //if(env->door[1]) env->door[1] += (alarm >> j++) & 1;
    } else  ret = false;
    //cout << ret << env->isInsert[2] << env->isInsert[3];

    return ret;
}

bool App_SensorBox::box_readData()
{
    bool res = false;
    uchar cmd[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x0B, 0x04, 0x0D};
    QByteArray recv = mSerial->transmit(cmd, sizeof(cmd));
    if(recv.size() > 20) res = box_recvPacket(recv);
    int t = 0; if(cm::runTime() > 72*60*60 ) {
        t = QRandomGenerator::global()->bounded(2365);
    } cm::mdelay(t + 1365);
    return res;
}

void App_SensorBox::box_offline()
{
    ushort isInsert = 0, k = 0;
    sEnvData *env = &(cm::masterDev()->env);
    env->isInsert[2] = (isInsert >> k++) & 1;
    env->isInsert[3] = (isInsert >> k++) & 1;
    env->smoke[0] = (isInsert >> k++) & 1;
    env->water[0] = (isInsert >> k++) & 1;
    //env->door[0] = (isInsert >> k++) & 1;
    //env->door[1] = (isInsert >> k++) & 1;
}

bool App_SensorBox::box_recvPacketii(const QByteArray &array)
{
    bool ret = true;
    uchar *ptr = (uchar *)array.data();
    sEnvData *env = &(cm::masterDev()->env);
    if((*ptr++ == 0x01) && (*ptr++ == 0x03))  {
        int len = *ptr; //cout << len;
        if(len == 84) ptr += 1; else return false;
        ushort isInsert = getShort(ptr); ptr += 2;
        env->tem.value[2] = getShort(ptr); ptr += 2;
        env->hum.value[2] = getShort(ptr); ptr += 2;
        env->tem.value[3] = getShort(ptr); ptr += 2;
        env->hum.value[3] = getShort(ptr); ptr += 2;
        ptr += 8*2; int k=0, j=4;// 报警上下限
        ushort alarm = getShort(ptr); //ptr += 2;

        //env->door[1] = (isInsert >> k++) & 1;
        env->reserve[0][0] = (isInsert >> k++) & 1;
        env->water[0] = (isInsert >> k++) & 1;
        env->smoke[0] = (isInsert >> k++) & 1;
        env->isInsert[3] = (isInsert >> k++) & 1;
        env->isInsert[2] = (isInsert >> k++) & 1;
        //        env->door[0] = (isInsert >> k++) & 1;
        //        env->door[1] = (isInsert >> k++) & 1;

        //if(env->tem.value[2]) env->tem.value[2] -= 400;
        //if(env->tem.value[3]) env->tem.value[3] -= 400;
        if(env->smoke[0]) env->smoke[0] += (alarm >> j) & 1;
        j++;
        if(env->water[0]) env->water[0] += (alarm >> j) & 1;
        j++;
        if(env->reserve[0][0]) env->door[1] = env->reserve[0][0]+((alarm >> j) & 1);
        cout<<" alram "<<alarm;
        j++;
        //        if(env->door[0]) env->door[0] += (alarm >> j++) & 1;
        //        if(env->door[1]) env->door[1] += (alarm >> j++) & 1;
    } else  ret = false;
    //cout << ret << env->isInsert[2] << env->isInsert[3];

    return ret;
}

bool App_SensorBox::box_readDataii()
{
    bool res = false;
    uchar cmd[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x2A, 0xC4, 0x15};
    QByteArray recv = mSerial->transmit(cmd, sizeof(cmd));
    if(recv.size() > 80) res = box_recvPacketii(recv);
    int t = 0; if(cm::runTime() > 72*60*60 ) {
        t = QRandomGenerator::global()->bounded(2365);
    } cm::mdelay(t + 1365);
    return res;
}

void App_SensorBox::box_offlineii()
{
    ushort isInsert = 0, k = 0;
    sEnvData *env = &(cm::masterDev()->env);
    env->reserve[0][0] = (isInsert >> k++) & 1;
    env->water[0] = (isInsert >> k++) & 1;
    env->smoke[0] = (isInsert >> k++) & 1;
    env->isInsert[3] = (isInsert >> k++) & 1;
    env->isInsert[2] = (isInsert >> k++) & 1;

    //    env->door[0] = (isInsert >> k++) & 1;
    //    env->door[1] = (isInsert >> k++) & 1;
}

void App_SensorBox::sensorBox_run()
{
    while(box_isRun) {
        if(cm::masterDev()->cfg.param.sensorBoxEn) {
            bool ret = box_open();
            if(ret){
                //                if(cm::masterDev()->cfg.nums.reserve[2] == 0){
                //                    for(int i=0; i<3; ++i) {
                //                        ret = box_readData(); if(ret) break;
                //                    } if(!ret) box_offline();
                //                }else{
                for(int i=0; i<3; ++i) {
                    ret = box_readDataii(); if(ret) break;
                } if(!ret) box_offlineii();
                //                }
            }
        } else {
            box_offlineii();
            box_close();
            cm::mdelay(360);
        }
    }
}
