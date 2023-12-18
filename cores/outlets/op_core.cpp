/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "op_core.h"

OP_Core::OP_Core(QObject *parent)
    : OP_ProRtu{parent}
{
    qint32 baudRate = QSerialPort::Baud19200;
#if (QT_VERSION > QT_VERSION_CHECK(5,15,0))
    bool ret = openSerial("/dev/ttyUSB0", baudRate);
#else
    bool ret = openSerial("/dev/ttyS5", baudRate);  /*打开串口并设置波特率19200*/
#endif
    if(ret) QtConcurrent::run(this,&OP_Core::run);
}

OP_Core *OP_Core::bulid(QObject *parent)
{
    static OP_Core* sington = nullptr;
    if(sington == nullptr) sington = new OP_Core(parent);
    return sington;
}

void OP_Core::run()
{
    cm::mdelay(1240);
    while (isRun) {
        bool ret = 0;
        int size = mDev->cfg.nums.boardNum; // cout << size;    /*获取执行版数量*/
        if(1 == mDev->cfg.param.devSpec) {  /*A系列*/
            cmsWrite(175); ota_updates();   /*将命令写入串口，并等待发送完成，执行OTA更新操作*/
            ret |= loop_readData(); /*获取回路数据*/            
        } else if(size) {
            for(int i=0; i<size; ++i) {
                cmsWrite(175);  /*将命令写入串口，并等待发送完成*/
                ota_updates();  /*执行OTA更新的操作*/
                ret |= readData(i+1);
            }
        }  cm::mdelay(10);
        if(!ret && (mDev->dtc.fault < 5)) mDev->dtc.fault += 1;
    }
}
