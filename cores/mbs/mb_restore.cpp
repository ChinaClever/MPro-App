/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "mb_restore.h"

Mb_Restore::Mb_Restore(QObject *parent)
    : Mb_Control{parent}
{

}

void Mb_Restore::mbUpdates()
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
