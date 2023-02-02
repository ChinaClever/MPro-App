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
    line_update();
    loop_update();
    output_update();
    env_update();


    upDevInfo();
    mbSetUpdate();
    mbAlarmUpdate();
    mbCtrlUpdate();
}
