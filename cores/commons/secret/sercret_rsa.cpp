/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "sercret_rsa.h"

Sercret_Rsa::Sercret_Rsa()
{

}


Sercret_Rsa *Sercret_Rsa::bulid()
{
    static Sercret_Rsa* sington = nullptr;
    if(sington == nullptr) {
        sington = new Sercret_Rsa();
    }
    return sington;
}

QByteArray Sercret_Rsa::rsa_sign(QByteArray rawData, const QByteArray &privKey, QRSAEncryption::Rsa rsa)
{
    return QRSAEncryption::signMessage(rawData, privKey, rsa);
}

bool Sercret_Rsa::rsa_checkSign(const QByteArray &rawData, const QByteArray &pubKey, QRSAEncryption::Rsa rsa)
{
    return QRSAEncryption::checkSignMessage(rawData, pubKey, rsa);
}

QByteArray Sercret_Rsa::rsa_encode(const QByteArray &rawData, const QByteArray &pubKey, QRSAEncryption::Rsa rsa)
{
    return  QRSAEncryption::encode(rawData, pubKey, rsa);
}

QByteArray Sercret_Rsa::rsa_decode(const QByteArray &rawData, const QByteArray &privKey, QRSAEncryption::Rsa rsa)
{
    return QRSAEncryption::decode(rawData, privKey, rsa);
}

bool Sercret_Rsa::generatePairKey(QByteArray &pubKey, QByteArray &privKey, const QByteArray& genesis, QRSAEncryption::Rsa rsa)
{
    return QRSAEncryption::generatePairKey(pubKey, privKey, genesis, rsa);
}
