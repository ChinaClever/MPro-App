#ifndef SERCRET_RSA_H
#define SERCRET_RSA_H
#include "RSA/qrsaencryption.h"
#include "sercret_aes.h"

struct sRsaIt {
    sRsaIt():rsa(QRSAEncryption::Rsa::RSA_2048), blockSizeMode(QRSAEncryption::BlockSize::Auto){}
    QByteArray pubKey;      // 公钥
    QByteArray privKey;     // 私钥
    QByteArray genesis;     // 私钥密码
    QRSAEncryption::Rsa rsa; // 密钥长度
    QRSAEncryption::BlockSize blockSizeMode; // 密钥格式
};

class Sercret_Rsa : public Sercret_Aes
{
public:
    Sercret_Rsa();
    static sRsaIt rsaCfg;
    bool rsa_generatePairKey();
    QByteArray rsa_sign(const QByteArray &rawData);
    bool rsa_checkSign(const QByteArray &rawData);
    QByteArray rsa_encode(const QByteArray &rawData);
    QByteArray rsa_decode(const QByteArray &rawData);
};

#endif // SERCRET_RSA_H
