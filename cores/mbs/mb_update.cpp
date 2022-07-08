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

void Mb_Update::mbUpdates()
{
    upDevInfo();
    mbLineUpdate();
    mbLoopUpdate();
    mbOutputUpdate();
    mbEnvUpdate();
    mbSetUpdate();
    mbAlarmUpdate();
    mbCtrlUpdate();
}
