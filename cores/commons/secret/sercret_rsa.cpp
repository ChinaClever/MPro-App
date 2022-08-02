/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "sercret_rsa.h"

sRsaIt Sercret_Rsa::rsaCfg;
Sercret_Rsa::Sercret_Rsa()
{

}

QByteArray Sercret_Rsa::rsa_sign(const QByteArray &rawData)
{
    return QRSAEncryption::signMessage(rawData, rsaCfg.privKey, rsaCfg.rsa);
}

bool Sercret_Rsa::rsa_checkSign(const QByteArray &rawData)
{
    return QRSAEncryption::checkSignMessage(rawData, rsaCfg.pubKey, rsaCfg.rsa);
}

QByteArray Sercret_Rsa::rsa_encode(const QByteArray &rawData)
{
    return  QRSAEncryption::encode(rawData, rsaCfg.pubKey, rsaCfg.rsa, rsaCfg.blockSizeMode);
}

QByteArray Sercret_Rsa::rsa_decode(const QByteArray &rawData)
{
    return QRSAEncryption::decode(rawData, rsaCfg.privKey, rsaCfg.rsa, rsaCfg.blockSizeMode);
}

bool Sercret_Rsa::rsa_generatePairKey()
{
    return QRSAEncryption::generatePairKey(rsaCfg.pubKey, rsaCfg.privKey, rsaCfg.genesis, rsaCfg.rsa);
}
