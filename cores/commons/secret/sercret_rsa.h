#ifndef SERCRET_RSA_H
#define SERCRET_RSA_H
//#include "RSA/qrsaencryption.h"
#include "sercet_tlscert.h"
#include "sercret_aes.h"

struct sRsaIt {
    sRsaIt():padding("PKCS1"), length(4096){}
    QByteArray pubKey;      // 公钥
    QByteArray privKey;     // 私钥
    QByteArray padding;     // 填充格式
    int length; // 密钥长度
};

class Sercret_Rsa : public Sercret_Aes
{
public:
    Sercret_Rsa();
    static sRsaIt rsaCfg;
    QByteArray rsa_encode(const QByteArray &rawData);
    QByteArray rsa_decode(const QByteArray &rawData);
    void rsa_test();

private:
    QByteArray rsaPubEncrypt(const QByteArray &clear_text, const QByteArray &pub_key);
    QByteArray rsaPriDecrypt(const QByteArray &cipher_text, const QByteArray &pri_key);
};

#endif // SERCRET_RSA_H
