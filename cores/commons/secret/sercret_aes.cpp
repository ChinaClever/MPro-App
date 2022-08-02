/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "sercret_aes.h"

sAesIt Sercret_Aes::aesCfg;
Sercret_Aes::Sercret_Aes()
{
}

QByteArray Sercret_Aes::aes_crypt(const QByteArray &rawText)
{
    return QAESEncryption::Crypt(aesCfg.level, aesCfg.mode, rawText, aesCfg.key, aesCfg.iv, aesCfg.padding);
}

QByteArray Sercret_Aes::aes_decrypt(const QByteArray &rawText)
{
    QByteArray decode = QAESEncryption::Decrypt(aesCfg.level, aesCfg.mode, rawText, aesCfg.key, aesCfg.iv, aesCfg.padding);
    return QAESEncryption::RemovePadding(decode);
}
