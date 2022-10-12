/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_updater.h"

Set_Updater::Set_Updater()
{

}

QVariant Set_Updater::otaStatus(int fc)
{
    sOtaUpdater *ota = &(cm::dataPacket()->ota);
    QVariant res; switch (fc) {
    case 1: res = ota->isRun;  break;
    case 2: res = ota->subId;  break;
    case 3: res = ota->progress;  break;
    case 4: res = ota->isOk;  break;
    case 5: res = ota->host;  break;
    default: cout << fc; break;
    }
    return res;
}

bool Set_Updater::ota_updater(int fc)
{
    cout << fc;

    return true;
}
