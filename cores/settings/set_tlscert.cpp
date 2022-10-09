/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "set_tlscert.h"
#include "sercret_core.h"

Set_TlsCert::Set_TlsCert()
{

}


QVariant Set_TlsCert::getTlsCert(uchar fc)
{
    QVariant res;
    Sercret_TlsCert *it = Sercret_TlsCert::bulid();
    if(fc > 20) res = it->subjectInfo(fc-20);
    else if(fc > 10) res = it->issuerInfo(fc-10);
    else {
        switch (fc) {
        case 1: res = QString(it->serialNumber());break;
        case 2: res = it->publicKey().length(); break;
        case 3: res = it->effectiveDate(); break;
        case 4: res = it->expiryDate(); break;
        default: qDebug() << Q_FUNC_INFO << fc; break;
        }
    }
    return res;
}

bool Set_TlsCert::setTlsCert(uchar fc, const QVariant &v)
{
    qDebug() << Q_FUNC_INFO << fc << v;
    return false;
}
