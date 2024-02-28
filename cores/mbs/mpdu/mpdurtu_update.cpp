/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mpdurtu_update.h"

Mpdurtu_Update::Mpdurtu_Update(QObject *parent)
    : Mpdurtu_Control{parent}
{

}

void Mpdurtu_Update::mbUpdates(int addr)
{
    mDevData = cm::devData(addr);
    upDevInfo();output_Updatesw();line_Update();loop_Update();
    output_Update();env_Update();
    QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
}
