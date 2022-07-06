/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "integr_core.h"

Integr_Core::Integr_Core(QObject *parent)
    : Integr_Receiver{parent}
{

}


Integr_Core *Integr_Core::bulid(QObject *parent)
{
    static Integr_Core* sington = NULL;
    if(sington == NULL) {
        sington = new Integr_Core(parent);
    }
    return sington;
}
