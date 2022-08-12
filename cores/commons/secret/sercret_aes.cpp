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

void Sercret_Aes::aes_setMode(int id)
{
    switch(id){
    case 0: aesCfg.mode = QAESEncryption::Mode::ECB;
    case 1: aesCfg.mode = QAESEncryption::Mode::CBC;
    case 2: aesCfg.mode = QAESEncryption::Mode::CFB;
    case 3: aesCfg.mode = QAESEncryption::Mode::OFB;
    }
}

void Sercret_Aes::aes_setPadding(int id)
{
    switch(id){
    case 0: aesCfg.padding = QAESEncryption::Padding::ZERO;
    case 1: aesCfg.padding = QAESEncryption::Padding::PKCS7;
    case 2: aesCfg.padding = QAESEncryption::Padding::ISO;
    }
}

void Sercret_Aes::aes_setLevel(int id)
{
    switch(id){
    case 0: aesCfg.level = QAESEncryption::Aes::AES_128;
    case 1: aesCfg.level = QAESEncryption::Aes::AES_192;
    case 2: aesCfg.level = QAESEncryption::Aes::AES_256;
    }
}
