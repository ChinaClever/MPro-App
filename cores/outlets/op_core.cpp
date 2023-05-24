/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "op_core.h"

OP_Core::OP_Core(QObject *parent)
    : OP_ZRtu{parent}
{
    qint32 baudRate = QSerialPort::Baud19200;
#if (QT_VERSION > QT_VERSION_CHECK(5,15,0))
    bool ret = openSerial("/dev/ttyUSB0", baudRate);
#else
    bool ret = openSerial("/dev/ttyS5", baudRate);
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
        int size = mDev->cfg.nums.boardNum; // cout << size;
        if(1 == mDev->cfg.param.devSpec) {
            ret |= loop_readData();
        } else if(size) {
            for(int i=0; i<size; ++i) {
                cmsWrite(175);
                ota_updates();
                ret |= readData(i+1);
            }
        }  cm::mdelay(10);
        if(ret) mDev->dtc.fault = 1;
    }
}
