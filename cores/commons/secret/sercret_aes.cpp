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
    QByteArray res; if(aesCfg.key.size()) {
        res = QAESEncryption::Crypt(aesCfg.level, aesCfg.mode, rawText,
                                    aesCfg.key, aesCfg.iv, aesCfg.padding);
    }
    return res;
}

QByteArray Sercret_Aes::aes_decrypt(const QByteArray &rawText)
{
    QByteArray res; if(aesCfg.key.size()) {
        QByteArray decode = QAESEncryption::Decrypt(aesCfg.level, aesCfg.mode, rawText,
                                                    aesCfg.key, aesCfg.iv, aesCfg.padding);
        res = QAESEncryption::RemovePadding(decode);
    }
    return res;
}
