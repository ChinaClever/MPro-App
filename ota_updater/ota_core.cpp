/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ota_core.h"

Ota_Core::Ota_Core(QObject *parent)
    : Ota_Web{parent}
{

}

Ota_Core *Ota_Core::bulid(QObject *parent)
{
    static Ota_Core* sington = nullptr;
    if(sington == nullptr) {
        sington = new Ota_Core(parent);
    }
    return sington;
}
