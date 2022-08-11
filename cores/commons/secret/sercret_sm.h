#ifndef SERCRET_SM_H
#define SERCRET_SM_H
#include "SMX/simplesm4.h"
#include "sercret_rsa.h"

// 支持ECB和CBC模式, PKCS7Padding补全
struct sSm4It {
    sSm4It(): type("sms4-cbc"), padding("PKCS7Padding"){}
    QByteArray type; // 支持ECB和CBC模式
    QByteArray padding;
    QByteArray key; // KEY密钥 16字节
    QByteArray iv; // 偏移量   //ECB模式同上, 只是不需要设置IV
};

class Sercret_SM : public Sercret_Rsa
{
public:
    Sercret_SM();
    static sSm4It sm4Cfg;
    QByteArray sm4_encrypt(const QByteArray &txt);
    QByteArray sm4_decrypt(const QByteArray &txt);
    void sm4_test();
};

#endif // SERCRET_SM_H
