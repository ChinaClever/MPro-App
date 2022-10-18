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
    //bool ret = openSerial("/dev/ttyUSB0", baudRate);
    bool ret = false;
#else
    bool ret = openSerial("/dev/ttyS1", baudRate);
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
    while (isRun) {
        int size = mDev->cfg.nums.boardNum;
        cout << size;
        for(int i=0; i<size; ++i) {
            cmsWrite(175);
            ota_updates();
            readData(i+1);
        } cm::mdelay(10);
    }
}
