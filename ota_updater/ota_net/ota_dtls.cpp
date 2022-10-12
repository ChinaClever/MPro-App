/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "ota_dtls.h"

Ota_Dtls::Ota_Dtls(QObject *parent)
    : Ota_Obj{parent}
{    
    qRegisterMetaType<sOtaFile>("sOtaFile");
    mDtls = Dtls_Recver::bulid(this);
    mDtls->listen();
}
