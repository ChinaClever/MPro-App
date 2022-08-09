/*
 *
 *  Created on: 2022年10月1日
 *      Author: Pmd
 */
#include "web_core.h"

Web_Core::Web_Core()
{

}


Web_Core *Web_Core::bulid()
{
    static Web_Core* sington = nullptr;
    if(sington == nullptr) sington = new Web_Core();
    return sington;
}

