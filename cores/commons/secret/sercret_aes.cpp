#include "sercret_aes.h"

Sercret_Aes::Sercret_Aes()
{
}

Sercret_Aes *Sercret_Aes::bulid()
{
    Sercret_Aes* sington = nullptr;
    if(sington == nullptr) {
        sington = new Sercret_Aes();
    }
    return sington;
}

QByteArray Sercret_Aes::aes_crypt(const QByteArray &rawText, const sAesIt &it)
{
    return QAESEncryption::Crypt(it.level, it.mode, rawText, it.key, it.iv, it.padding);
}

QByteArray Sercret_Aes::aes_decrypt(const QByteArray &rawText, const sAesIt &it)
{
    QByteArray decode = QAESEncryption::Decrypt(it.level, it.mode, rawText, it.key, it.iv, it.padding);
    return QAESEncryption::RemovePadding(decode);
}