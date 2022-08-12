/*
 *
 *  Created on: 2022年10月1日
 *      Author: Lzy
 */
#include "sercret_sm.h"

sSm4It Sercret_SM::sm4Cfg;
Sercret_SM::Sercret_SM()
{

}

void Sercret_SM::sm4_test()
{
    sm4Cfg.key = "1234567890123456";
    sm4Cfg.iv = "asdfghjklzxcvbnm";
    qDebug() << sm4_decrypt(sm4_encrypt("hello sm4!")); //加密之后再解密
}

QByteArray Sercret_SM::sm4_encrypt(const QByteArray &txt)
{
    QByteArray res;
    if(sm4Cfg.key.size()) {
        SimpleSm4 s;
        s.setType(SimpleSm4::CBC);
        s.setKey(sm4Cfg.key);
        s.setIv(sm4Cfg.iv);
        res =s.encrypt(txt);
    }
    return res;
}

QByteArray Sercret_SM::sm4_decrypt(const QByteArray &txt)
{
    QByteArray res;
    if(sm4Cfg.key.size()) {
        SimpleSm4 s;
        s.setType(SimpleSm4::CBC);
        s.setKey(sm4Cfg.key);
        s.setIv(sm4Cfg.iv);
        res = s.decrypt(txt);
    }
    return res;
}
