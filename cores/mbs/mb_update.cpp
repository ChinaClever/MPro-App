/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mb_update.h"

Mb_Update::Mb_Update(QObject *parent)
    : Mb_Control{parent}
{

}

void Mb_Update::mbUpdates(int addr)
{
    mStartReg = addr*MbReg_MaxSize;
    mDevData = cm::devData(addr);
    upDevInfo(); line_update();
    loop_update(); output_update();
    group_update(); env_update(); mbSetUpdate();
    QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
}
