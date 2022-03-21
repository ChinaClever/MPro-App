#ifndef SERCRET_SMX_H
#define SERCRET_SMX_H
#include "sercret_rsa.h"

class Sercret_Smx : public Sercret_Rsa
{
public:
    Sercret_Smx();
    static Sercret_Smx *bulid();
    QString sm2_MakePubKey(const QByteArray &priKey); // 生成公钥

    // 签名
    QString sm2_Sign(const QByteArray &msg, const QByteArray &user,
                  const QByteArray &pubKey, const QByteArray &priKey);

    // 验签
    bool sm2_Verify(const QByteArray &pubKey, const QByteArray &Sign,
                    const QString &plain, const QByteArray &user);

    // 加密
    QString sm2_Encrypt(const QByteArray &raw, const QByteArray &pubKey, uint PlainFlag=0);

    // 解密
    QString sm2_Decrypt(const QByteArray &raw, const QByteArray &priKey, uint PlainFlag=0);

    // 3次摘要计算
    QString sm3(const QByteArray &raw);
    QString sm3_2(const QByteArray &pubKey, const QByteArray &user);
    QString sm3_3(const QByteArray &hash1, const QByteArray &hash2);

    // 非对称加密
    QString sm4_crypt(const QByteArray &raw, const QByteArray &Key);
    QString sm4_decrypt(const QByteArray &raw, const QByteArray &Key);

    // 字符串处理
    QString utils_Unfold(const QByteArray &raw); // 字符展开
    QString utils_Fold(const QByteArray &raw); // 字符压缩
};

using Sercrets = Sercret_Smx;

#endif // SERCRET_SMX_H
