#ifndef SERCRET_AES_H
#define SERCRET_AES_H
#include "AES/qaesencryption.h"

struct sAesIt {
    sAesIt():level(QAESEncryption::AES_256), mode(QAESEncryption::CBC), padding(QAESEncryption::ISO){}
    QAESEncryption::Aes level; // AES密钥长度 加密级别
    QAESEncryption::Mode mode; // AES加密模式 操作模式
    QAESEncryption::Padding padding; // 填充方法
    QByteArray key; // KEY密钥
    QByteArray iv; // 偏移量
};

class Sercret_Aes
{
public:
    Sercret_Aes();
    static sAesIt aesCfg;
    QByteArray aes_crypt(const QByteArray &rawText);
    QByteArray aes_decrypt(const QByteArray &rawText);
};

#endif // SERCRET_AES_H
