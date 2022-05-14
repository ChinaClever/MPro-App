/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_core.h"

Set_Core::Set_Core()
{
    Set_ReadWrite::bulid();
}

Set_Core *Set_Core::bulid()
{
    static Set_Core* sington = nullptr;
    if(sington == nullptr) {
        sington = new Set_Core();
    }
    return sington;
}

void Set_Core::writeSettings()
{
    Set_ReadWrite::bulid()->writeSettings();
}
