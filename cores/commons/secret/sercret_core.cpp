/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "sercret_core.h"

sSercretIt Sercret_Core::cfg;
Sercret_Core::Sercret_Core()
{

}

Sercret_Core *Sercret_Core::bulid()
{
    static Sercret_Core* sington = NULL;
    if(sington == NULL) {
        sington = new Sercret_Core();
    }
    return sington;
}

QByteArray Sercret_Core::encode(const QByteArray &raw)
{
    QByteArray res;
    switch (cfg.type) {
    case 0: res = raw; break;
    case 1: res = aes_crypt(raw); break;
    case 2: res = rsa_encode(raw); break;
    case 3: res = sm4_encrypt(raw); break;
    default: qDebug() << Q_FUNC_INFO; break;
    }
    return res;
}

QByteArray Sercret_Core::decrypt(const QByteArray &raw)
{
    QByteArray res;
    switch (cfg.type) {
    case 0: res = raw; break;
    case 1: res = aes_decrypt(raw); break;
    case 2: res = rsa_decode(raw); break;
    case 3: res = sm4_decrypt(raw); break;
    default: qDebug() << Q_FUNC_INFO; break;
    }
    return res;
}

QByteArray Sercret_Core::base64_encode(const QByteArray &raw)
{
    return raw.toBase64();
}

QByteArray Sercret_Core::base64_decode(const QByteArray &raw)
{
    return QByteArray::fromBase64(raw);
}
