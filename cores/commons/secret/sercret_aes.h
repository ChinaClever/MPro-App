#ifndef SERCRET_AES_H
#define SERCRET_AES_H
#include "AES/qaesencryption.h"

struct sAesIt {
    sAesIt():level(QAESEncryption::AES_256), mode(QAESEncryption::CBC), padding(QAESEncryption::ISO){}
    QAESEncryption::Aes level;
    QAESEncryption::Mode mode;
    QAESEncryption::Padding padding;
    QByteArray key;
    QByteArray iv;
};

class Sercret_Aes
{
public:
    Sercret_Aes();
    static Sercret_Aes *bulid();
    QByteArray aes_crypt(const QByteArray &rawText, const sAesIt &it);
    QByteArray aes_decrypt(const QByteArray &rawText, const sAesIt &it);
};

#endif // SERCRET_AES_H
