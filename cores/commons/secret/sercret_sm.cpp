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

QByteArray Sercret_SM::sm4_encrypt(const QByteArray &txt)
{
    QByteArray res;
    if(sm4Cfg.key.size()) {
        SimpleSm4 s;
        s.setType(sm4Cfg.type);
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
        s.setType(sm4Cfg.type);
        s.setKey(sm4Cfg.key);
        s.setIv(sm4Cfg.iv);
        res = s.decrypt(txt);
    }
    return res;
}
